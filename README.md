# Iris

This is a CPU-based path tracer.

## Features

- Interactive - Users can move around the scene with the arrow keys and mouse. <br>
- Multithreaded - Sampling is distributed among multiple processor threads. <br>
- Next Event Estimation - Direct light is sampled at each light bounce using a monte carlo method to estimate occluders. <br>
- 3D Models - Iris can render 3D models however due to the high amount of computations required and that Iris runs on the CPU, this can be very slow depending on the model's triangle count. <br>

## Gallery

512 Samples with a max path depth of 8 and no direct light sampling:
![512samples-8depth-0ds-demo4](https://user-images.githubusercontent.com/35866844/52524068-924af280-2c4d-11e9-8d52-190e330196fa.PNG)

512 Samples with a max path depth of 8 with 1 direct light sample:
![512samples-8depth-1ds-demo4](https://user-images.githubusercontent.com/35866844/52599367-4ba3f680-2e0d-11e9-836a-8e6ad627452f.PNG)

## Controls
- ESC key closes the application.<br>
- Keys W, A, S, D used to move forward, left, backward, and right respectively.<br>
- The <u>space</u> key moves the camera up and the <u>ctrl</u> key moves the camera down.<br>
- Camera orientation is controlled with the mouse <u>while</u> holding the left mouse button down. This is to prevent accidental loss of samples as when the scene or camera orientation/position is changed, all previous samples are flushed.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

