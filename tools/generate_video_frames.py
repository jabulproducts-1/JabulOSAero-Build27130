import sys
import os
import cv2

if len(sys.argv) != 3:
    print("Usage: generate_video_frames.py <input.mp4> <out_dir>")
    sys.exit(1)

input_file = sys.argv[1]
out_dir = sys.argv[2]

cap = cv2.VideoCapture(input_file)
if not cap.isOpened():
    print("Could not open video")
    sys.exit(1)

count = 0
frame_idx = 0
while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    # We want ~10 FPS, original is 30 FPS, so keep every 3rd frame
    if count % 3 == 0:
        # Resize to 512x384
        h, w = frame.shape[:2]
        scale = min(512/w, 384/h)
        nw, nh = int(w*scale), int(h*scale)
        resized = cv2.resize(frame, (nw, nh), interpolation=cv2.INTER_AREA)
        
        # Create a black canvas
        canvas = cv2.copyMakeBorder(resized, 
                                    (384-nh)//2, 384-nh - (384-nh)//2,
                                    (512-nw)//2, 512-nw - (512-nw)//2, 
                                    cv2.BORDER_CONSTANT, value=(0,0,0))
        
        out_path = os.path.join(out_dir, f"frame_{frame_idx:03d}.bmp")
        # Ensure 24-bit BMP by writing with cv2.imwrite
        cv2.imwrite(out_path, canvas)
        frame_idx += 1
    
    count += 1

cap.release()
