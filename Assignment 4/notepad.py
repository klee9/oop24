import os
import sys
import subprocess

def install_dependencies(requirements_file):
    '''
    Install all dependencies

    Args:
        requirements_file (string): name of the requirements file 
    '''
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "install", "-r", requirements_file])
        print("Dependencies installed successfully.")
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")

# install dependencies
# install_dependencies("requirements.txt")

import cv2
import numpy as np
import mediapipe as mp
import fitz
from PIL import Image
from io import BytesIO
from reportlab.pdfgen import canvas as canv

class Webcam:
    '''
    A class for handling webcam-related actions
    '''
    def __init__(self, camera_idx=0, width=840, height=480):
        '''
        Initialize the Webcam

        Args: 
            camera_idx (int):
            width (int): The width of the webcam feed
            height(int): The height of the webcam feed
        '''
        self.cam = cv2.VideoCapture(camera_idx)
        self.cam.set(cv2.CAP_PROP_FRAME_WIDTH, width)
        self.cam.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

    def get_frame(self):
        '''
        Get webcam frame

        Returns:
            numpy.ndarray: The flipped(mirror-view) webcam feed
        '''
        ret, frame = self.cam.read()
        if not ret:
            return None

        return cv2.flip(frame, 1)

    def release(self):
        '''
        Close webcam
        '''
        self.cam.release()
        cv2.destroyAllWindows()

class HandDetector:
    '''
    A class for handling hand detection and tracking
    '''
    def __init__(self, max_hands=1, detection_conf=0.6, tracking_conf=0.6):
        '''
        Initialize the Hand Detector

        Args:
            max_hands (int): The maximum number of hands to track
            detection_conf (float): The confidence threshold of hand detection
            tracking_conf (float): The confidence threshold of hand tracking
        '''
        self.hands = mp.solutions.hands.Hands(
            static_image_mode=False,
            max_num_hands=max_hands,
            min_detection_confidence=detection_conf,
            min_tracking_confidence=tracking_conf
        )
        self.brush = mp.solutions.drawing_utils

    def find_hands(self, frame):
        '''
        Detect hands and return results

        Args:
            frame (numpy.ndarray): The frame of the webcam feed

        Returns:
            mediapipe.python.solutions.hands.HandsResult: The results of processed data
        '''
        rgb_f = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        results = self.hands.process(rgb_f)
        return results

    def draw_landmarks(self, frame, hand_landmarks):
        '''
        Draw and return hand landmarks

        Args:
            frame (numpy.ndarray): The frame of the webcam feed
            hand_landmarks (list): Detected landmarks
        '''
        if hand_landmarks:
            for handLms in hand_landmarks:
                # draw the landmarks on the frame
                self.brush.draw_landmarks(
                    frame,
                    handLms,
                    mp.solutions.hands.HAND_CONNECTIONS,
                    self.brush.DrawingSpec(color=(0, 255, 0), thickness=2, circle_radius=2),
                    self.brush.DrawingSpec(color=(255, 0, 0), thickness=2)
                )

class GestureHandler:
    def __init__(self, pinch_threshold=0.0020, swipe_threshold=0.05):
        self.pinch_threshold = pinch_threshold
        self.swipe_threshold = swipe_threshold
        self.previous_index_x = None

    def get_dist(self, thumb, index):
        return (thumb.x - index.x)**2 + (thumb.y - index.y)**2

    def is_pinching(self, thumb, index):
        dist = self.get_dist(thumb, index)
        return dist < self.pinch_threshold

    def swipe_left(self, thumb, index):
        if not self.is_pinching(thumb, index) and self.previous_index_x is not None:
            if self.previous_index_x - index.x > self.swipe_threshold:
                self.previous_index_x = index.x
                return True
        self.previous_index_x = index.x 
        return False

    def swipe_right(self, thumb, index):
        if not self.is_pinching(thumb, index) and self.previous_index_x is not None:
            if index.x - self.previous_index_x > self.swipe_threshold:
                self.previous_index_x = index.x 
                return True
        self.previous_index_x = index.x
        return False

class Eraser:
    '''
    A simple class for erasing
    '''
    def clear_screen(self, layer):
        '''
        Clear screen (erase everything on the screen)

        Args:
            layer (numpy.ndarray): A layer to be erased

        Returns:
            numpy.ndarray: The erased layer
        '''
        global lines
        layer[:] = 0
        lines = []
        return layer

    def erase_nearby(self, layer, cursor, threshold=10):
        '''
        Erases nearby lines

        Args:
            layer (numpy.ndarray): A layer to be erased
            cursor (tuple): The cursor being used in the program
            threshold (int): A threshold for cursor-line distance for erasing

        Returns:
            numpy.ndarray: The layer that was erased
        '''
        global lines
        if cursor is None:
            return layer

        # filter lines not near the cursor
        lines_to_keep = []
        for line in lines:
            start, end = line
            if not self.is_near_line(cursor, start, end, threshold):
                lines_to_keep.append(line)

        lines = lines_to_keep

        # clear and redraw
        layer[:] = 0
        for line in lines:
            cv2.line(layer, line[0], line[1], color, thickness=3)

        return layer

    def is_near_line(self, cursor, start, end, threshold=10):
        '''
        Checks if a line is nearby

        Args:
            cursor (tuple): The cursor being used in the program
            start (tuple): The starting point of the line
            end (tuple): The end point of the line
            threshold (int): The threshold for cursor-line distance

        Returns:
            bool: The comparison result of distance and threshold
        '''
        x0, y0 = cursor
        x1, y1 = start
        x2, y2 = end

        # calculate cursor-line distance
        num = abs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1)
        den = ((y2 - y1)**2 + (x2 - x1)**2)**0.5
        dist = num / den if den != 0 else float('inf')

        return dist <= threshold

class PDFHandler:
    '''
    A class for handling PDFs
    '''
    def __init__(self, page_num=0):
        '''
        Initializes the pdf Handler class
        
        Args:
            page_num (int): The page number of the pdf file
            eraser (Eraser): An object of the Eraser class
        '''
        self.page_num = page_num
        self.eraser = Eraser()

    def get_resolutions(self, pdf_path, dpi=72):
        '''
        Calculate and return the resolutions of the pdf file

        Args:
            pdf_path (string): The path of the pdf file
            dpi (int): The dpi value for changing resolutions of the pdf file

        Returns:
            tuple: The width and height of the pdf file
        '''
        doc = fitz.open(pdf_path)
        rect = doc[0].rect
        width = int(rect.width * dpi / 72)
        height = int(rect.height * dpi / 72)
        doc.close()

        return width, height

    # converts pdf to image (for adding annotations)
    def pdf2img(self, pdf_path, output_dir):
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
            print(f"Directory {output_dir} created.")

        doc = fitz.open(pdf_path)
        images = []
        
        for page_num in range(len(doc)):
            page = doc[page_num]
            pix = page.get_pixmap()
            output_path = f"{output_dir}/page_{page_num + 1}.png"
            pix.save(output_path)
            images.append(output_path)

        doc.close()
        return images

    # converts image to pdf
    def img2pdf(self, img_paths, output_pdf_path):
        c = canv.Canvas(output_pdf_path)
        for image_path in img_paths:
            # open the image
            img = Image.open(image_path)
            width, height = img.size

            # set the page size to match the image size
            c.setPageSize((width, height))
            c.drawImage(image_path, 0, 0, width, height)
            c.showPage()

        c.save()

    # saves the current page
    def save_page(self, page_path, pdf_layer, note_layer):
        b, g, r, a = cv2.split(note_layer)
        alpha = a / 255.0
        for c in range(3):
            pdf_layer[:, :, c] = ((1 - alpha) * pdf_layer[:, :, c] + alpha * note_layer[:, :, c])

        # save the updated image
        print(f"saving {page_path}")
        cv2.imwrite(page_path, pdf_layer)

    # returns next page
    def next_page(self, pdf_imgs, layer):
        self.eraser.clear_screen(layer)
        self.page_num = (self.page_num + 1) % len(pdf_imgs)
        img = cv2.imread(pdf_imgs[self.page_num])
        img = cv2.resize(img, self.get_resolutions(pdf_imgs[self.page_num]))
        return img.copy()

    # returns previous page
    def prev_page(self, pdf_imgs, layer):
        self.eraser.clear_screen(layer)
        self.page_num = (self.page_num - 1) % len(pdf_imgs)
        img = cv2.imread(pdf_imgs[self.page_num])
        img = cv2.resize(img, self.get_resolutions(pdf_imgs[self.page_num]))
        return img.copy()

# initialize files
pdf_path = input("PDF Path >> ") # "/Users/klee9/Desktop/oop.pdf"
output_dir = input("Save Path >> ") # "/Users/klee9/Desktop/pdf2img"
pdf_output_dir = pdf_path

# initialize classes
webcam = Webcam()
detector = HandDetector()
gesture = GestureHandler()
pdf = PDFHandler()
eraser = Eraser()

pdf_imgs = pdf.pdf2img(pdf_path, output_dir)
page_num = 0

# initialize graphics-related variables
webcam_width, webcam_height = pdf.get_resolutions(pdf_path)

img = cv2.imread(pdf_imgs[page_num])
img = cv2.resize(img, (webcam_width, webcam_height))

lines = []
prev_index = None
color = (0, 0, 0, 128)

pdf_layer = img.copy()
note_layer = np.zeros((pdf_layer.shape[0], pdf_layer.shape[1], 4), dtype=np.uint8)
pdf_layer = cv2.resize(pdf_layer, (webcam_width, webcam_height))
note_layer = cv2.resize(note_layer, (webcam_width, webcam_height))

# initialize color palette
colors = [
    (0, 0, 255, 128),     # red
    (0, 165, 255, 128),   # orange
    (0, 255, 255, 128),   # yellow
    (0, 255, 0, 128),     # green
    (255, 0, 0, 128),     # blue
    (130, 0, 75, 128),    # indigo
    (238, 130, 238, 128), # pink
    (255, 255, 255, 128), # white
    (0, 0, 0, 128),       # black
]

current_color_index = 0
color = colors[current_color_index]

# initalize kalman filter
kalman = cv2.KalmanFilter(4, 2)

kalman.transitionMatrix = np.array([
    [1, 0, 1, 0],  # x = x + vx
    [0, 1, 0, 1],  # y = y + vy
    [0, 0, 1, 0],  # vx = vx
    [0, 0, 0, 1]   # vy = vy
], dtype=np.float32)

kalman.measurementMatrix = np.array([
    [1, 0, 0, 0],  # measure x
    [0, 1, 0, 0]   # measure y
], dtype=np.float32)

# covariance matrices
kalman.processNoiseCov = np.eye(4, dtype=np.float32) * 0.03
kalman.measurementNoiseCov = np.eye(2, dtype=np.float32) * 1

# initial states 
kalman.statePre = np.zeros((4, 1), dtype=np.float32)
kalman.statePost = kalman.statePre.copy()

while True:
    base_layer = webcam.get_frame()
    if base_layer is None:
        break
    
    # get base layer dimensions
    base_height, base_width, _ = base_layer.shape

    # resize pdf and note layers to match the base layer
    pdf_layer = cv2.resize(pdf_layer, (base_width, base_height))
    note_layer = cv2.resize(note_layer, (base_width, base_height))

    # detect hands
    results = detector.find_hands(base_layer)
    cursor_coords = None
    
    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            # get coordinates
            index = hand_landmarks.landmark[mp.solutions.hands.HandLandmark.INDEX_FINGER_TIP]
            thumb = hand_landmarks.landmark[mp.solutions.hands.HandLandmark.THUMB_TIP]

            # convert normalized coordinates to pixel coordinates
            index_coords = np.array([[index.x * base_width], [index.y * base_height]], dtype=np.float32)
            thumb_coords = (int(thumb.x * base_width), int(thumb.y * base_height))

            kalman.correct(index_coords)
            prediction = kalman.predict()
            
            index_coords = (int(prediction[0]), int(prediction[1]))
            cursor_coords = index_coords
            
            # check for swiping gestures
            if gesture.swipe_left(thumb, index):
                current_color_index = (current_color_index - 1) % len(colors)
                color = colors[current_color_index]

            elif gesture.swipe_right(thumb, index):
                current_color_index = (current_color_index + 1) % len(colors)
                color = colors[current_color_index]

            # draw a line while pinching
            if prev_index and gesture.is_pinching(thumb, index):
                cv2.line(note_layer, prev_index, index_coords, color, thickness=3)
                lines.append((prev_index, index_coords))

            prev_index = index_coords

    # blend the note layer with the pdf layer
    b, g, r, a = cv2.split(note_layer)
    alpha = a / 255.0
    for c in range(3):
        pdf_layer[:, :, c] = ((1 - alpha) * pdf_layer[:, :, c] + alpha * note_layer[:, :, c])

    display_layer = pdf_layer.copy()

    # draw the cursor on the display layer
    if cursor_coords:
        cv2.circle(display_layer, cursor_coords, 5, color, -1)

    # display the composite frame with the cursor
    cv2.imshow("Interactive Notepad", display_layer)

    # handle key events
    key = cv2.waitKey(1) & 0xFF

    # c: clear the note layer
    if key == ord('c'): 
        note_layer = eraser.clear_screen(note_layer)
        pdf_layer = cv2.imread(pdf_imgs[page_num])

    # e: erase line closest to the cursor
    elif key == ord('e'):
        note_layer = eraser.erase_nearby(note_layer, cursor_coords, threshold=10)
        pdf_layer = cv2.imread(pdf_imgs[page_num])

    # n: go to the next page
    elif key == ord('n'):
        pdf.save_page(pdf_imgs[page_num], pdf_layer, note_layer)
        pdf_layer = pdf.next_page(pdf_imgs, note_layer)
        page_num = (page_num + 1) % len(pdf_imgs)

    # p: go to the previous page
    elif key == ord('p'):
        pdf.save_page(pdf_imgs[page_num], pdf_layer, note_layer)
        pdf_layer = pdf.prev_page(pdf_imgs, note_layer)
        page_num = (page_num - 1) % len(pdf_imgs)

    # change colors
    if ord('1') <= key <= ord('9'):
        current_color_index = key - ord('1')
        color = colors[current_color_index % len(colors)]
    
    # esc: exit
    elif key == 27:
        # save the last page, then convert the whole thing into pdf
        pdf.save_page(pdf_imgs[page_num], pdf_layer, note_layer)
        pdf.img2pdf(pdf_imgs, pdf_output_dir)
        break

webcam.release()
cv2.destroyAllWindows()
