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

# upon completion, import OpenCV and MediaPipe
import cv2
import mediapipe as mp

# webcam class
class Webcam:
    def __init__(self, camera_idx=0, width=640, height=480):
        # launch webcam
        cv2.VideoCapture(0)
        if not self.cam.isOpened():
            raise Exception("Could not open the webcam.")

        # set resolution
        self.cam.set(cv2.CAP_PROP_FRAME_WIDTH, width)
        self.cam.set(cv2.CAP_PROP_FRAME_HEIGHT, height)
    
    def get_frame(self):
        ret, frame = self.cam.read()
        if not ret:
            return None

        # return mirrored view
        return cv2.flip(frame, 1
        
    def release(self):
        self.cam.release()
        cv2.destroyAllWindows()

# hand gesture recognizer
class HandDetector(Webcam):
    def __init__(self, max_hands=1, detection_conf=0.6, tracking_conf=0.6):
        self.hands = mp.solutions.hands.Hands(
            static_image_mode=False,
            max_num_hands=max_hands,
            min_detection_confidence=detection_conf,
            min_tracking_confidence=tracking_conf
        )
        self.brush = mp.solutions.drawing_utils
        
    def find_hands(self, frame):
        rgb_f = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        results = self.hands.process(rgb_f)
        return results
    
    def find_fingertips(self, frame):

if __name__ == "__main__": 
    setup_venv()
    webcam = Webcam()
    webcam.show_feed()
