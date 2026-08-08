import os
import sys

import cv2


TARGET_WIDTH = 320
TARGET_HEIGHT = 180
TARGET_FPS = 30.0


def sanitize_media_token(path: str) -> str:
    stem = os.path.splitext(os.path.basename(path))[0].lower()
    result = []
    last_was_underscore = False

    for char in stem:
        if char.isalnum():
            result.append(char)
            last_was_underscore = False
        elif char in (" ", "-", "_"):
            if result and not last_was_underscore:
                result.append("_")
                last_was_underscore = True

    if result and result[-1] == "_":
        result.pop()

    token = "".join(result)
    return token or "media"


def write_letterboxed_frame(frame, out_path: str) -> None:
    height, width = frame.shape[:2]
    scale = min(TARGET_WIDTH / width, TARGET_HEIGHT / height)
    new_width = max(1, int(width * scale))
    new_height = max(1, int(height * scale))
    resized = cv2.resize(frame, (new_width, new_height), interpolation=cv2.INTER_AREA)

    top = (TARGET_HEIGHT - new_height) // 2
    bottom = TARGET_HEIGHT - new_height - top
    left = (TARGET_WIDTH - new_width) // 2
    right = TARGET_WIDTH - new_width - left

    canvas = cv2.copyMakeBorder(
        resized,
        top,
        bottom,
        left,
        right,
        cv2.BORDER_CONSTANT,
        value=(0, 0, 0),
    )
    cv2.imwrite(out_path, canvas)


def main() -> int:
    if len(sys.argv) != 3:
        print("Usage: generate_media_frames.py <input.mp4> <out_dir>")
        return 1

    input_file = sys.argv[1]
    out_dir = sys.argv[2]
    token = sanitize_media_token(input_file)

    cap = cv2.VideoCapture(input_file)
    if not cap.isOpened():
        print(f"Could not open video: {input_file}")
        return 1

    fps = cap.get(cv2.CAP_PROP_FPS)
    if fps <= 0.0:
        fps = TARGET_FPS
    frame_step = max(1, int(round(fps / TARGET_FPS)))

    frame_index = 0
    source_index = 0
    while True:
        ok, frame = cap.read()
        if not ok:
            break

        if source_index % frame_step == 0:
            out_name = f"jmv_{token}_{frame_index:03d}.bmp"
            write_letterboxed_frame(frame, os.path.join(out_dir, out_name))
            frame_index += 1
            if frame_index >= 999:
                break

        source_index += 1

    cap.release()
    print(f"generated {frame_index} frames for {os.path.basename(input_file)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
