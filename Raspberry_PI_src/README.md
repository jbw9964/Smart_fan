## This is a directory about `Raspberry PI`

---
## Operating System & Essential libraries

- `OS` : [`Ubuntu Desktop 22.04.3 LTS (64-bit)`](https://ubuntu.com/download/raspberry-pi)
    - Desktop OS with long-term support for RPi 4/400 models with 2Gb+

|Package|Description|Version|
|-------|---|------:|
|[`OpenCV-python`](https://github.com/opencv/opencv-python?tab=readme-ov-file)|OpenCV (Open Source Computer Vision Library: http://opencv.org) is an open-source BSD-licensed library that includes several hundreds of computer vision algorithms.|   4.8.1.78|
|[`Ultralytics`](https://github.com/ultralytics/ultralytics)|Ultralytics YOLOv8 is a cutting-edge, state-of-the-art (SOTA) model that builds upon the success of previous YOLO versions and introduces new features and improvements to further boost performance and flexibility.|   8.0.205|
|[`Pytorch`](https://github.com/pytorch/pytorch)|PyTorch is a Python package that provides Tensor computation (like NumPy) with strong GPU acceleration & Deep neural networks built on a tape-based autograd system|   2.0.1|
|[`Pyserial`](https://pyserial.readthedocs.io/en/latest/index.html)|This module encapsulates the access for the serial port.|    3.5|


---
## File description
1. [`best.pt`](./)  :   A `Yolov8n` model to use image prediction. It's already trained.

2. [`Raspi_Yolo.py`](./Raspi_Yolo.py)   :   A source code to execute `Raspberry PI`.

3. [`test`](./test/)    :   A directory that used to test some image, FPS of `Yolov8n`, and others.

---
## Raspberry PI control diagram - [`Raspi_Yolo.py`](./Raspi_Yolo.py)

<p align="center">
    <img src="../image/raspberry_diagram_full.drawio.png" style="height :500px" title="시연연상_고속"/>
</p>


---
## Whole file tree

```
[ 192]  .
├── [   0]  README.md
├── [ 11K]  Raspi_Yolo.py
├── [6.3M]  best.pt
└── [ 256]  test
    ├── [7.9K]  raspberry_pi_test.py
    ├── [298K]  test.png
    ├── [6.2M]  yolov8n.pt
    ├── [ 160]  test_frame
    │   ├── [4.2K]  test_frame.ipynb
    │   ├── [3.0K]  test_frame.py
    │   └── [5.2K]  test_frame_with_thread.py
    ├── [ 128]  test_serial
    │   ├── [3.3K]  test_serial.ipynb
    │   └── [2.2K]  test_serial.py
    └── [ 128]  test_window
        ├── [1.3K]  test_window.ipynb
        └── [ 743]  test_window.py

5 directories, 13 files
```