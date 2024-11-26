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

# open webcam
cam = cv2.VideoCapture(0)

if not cam.isOpened():
    print("Could not open the webcam.")
    exit()

while True:
    ret, frame = cam.read()

    if not ret: 
        print("Failed to grab frame")
        break

    cv2.imshow("Webcam Feed", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cam.release()
cv2.destroyAllWindows()
