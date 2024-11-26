import os
import sys
import subprocess

def setup_venv():
    try:
        # create the virtual environment
        env_path = "env"
        if not os.path.exists(env_path):
            print("Creating virtual environment...")
            subprocess.check_call([sys.executable, "-m", "venv", env_path])
        else:
            print("Virtual environment already exists.")

        # activate the virtual environment
        activate_script = os.path.join(venv_path, "bin", "activate")
        subprocess.check_call([sys.executable, "source", activate_script])

        # install required packages
        print("Installing dependencies...")
        subprocess.check_call([sys.executable, "-m", "pip", "install", "--upgrade", "pip"])
        subprocess.check_call([sys.executable, "-m", "pip", "install", "opencv-python", "opencv-python-headless", "mediapipe"])

        print("Setup complete. OpenCV and MediaPipe installed successfully!")

    except subprocess.CalledProcessError as e:
        print(f"Error during setup: {e}")
    except Exception as e:
        print(f"Unexpected error: {e}")

setup_venv()

# upon completion, import OpenCV and MediaPipe
import cv2
import mediapipe as mp

# webcam class
class Webcam:
    def __init__(self, camera_idx=0, width=640, height=480):
        """
        Initializes webcam
        :param camera_idx: index of the camera (0 for the default camera)
        :param width: width of the webcam feed
        :param heigth: height of the webcam feed
        """
        # launch webcam
        cv2.VideoCapture(0)
        if not self.cam.isOpened():
            raise Exception("Could not open the webcam.")

        # set resolution
        self.cam.set(cv2.CAP_PROP_FRAME_WIDTH, width)
        self.cam.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

    def get_frame(self):
        """
        Captures a frame
        :return: captured frame
        """
        ret, frame = self.cam.read()
        if not ret:
            raise Exception("Failed to capture frame.")
        return frame
        
    def show_feed(self, window_name="Webcam"):
        """
        Displays webcam feed
        :param window_name: name of the webcam window
        """
        while True:
            frame = self.get_frame()
            cv2.imshow(window_name, frame)

            if cv2.waitKey(1) & 0xff == ord('q'):
                break

            cam.release()
            cv2.destroyAllWindows()

# hand gesture recognizer
class GestureDetector:
    def __init__(self, recog=True):
        self.recog = recog
    def toggle(self):
        self.recog = !(self.recog)
    def detect(self):
        # add a function for detecting gestures

# painter
class Brush:

if __name__ == "__main__": 
    webcam = Webcam()
    webcam.show_feed()
