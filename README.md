# Iris

This is a CPU-based path tracer.

## Features

Interactive - Users can move around the scene with the arrow keys. <br>
Multithreaded - Sampling is distributed among multiple processor threads. <br>
Next Event Estimation - Direct light is sampled at each light bounce using a monte carlo method to estimate occluders. <br>
3D Models - Iris can render 3D models however due to the high amount of computations required and that Iris runs on the CPU, this can be very slow depending on the model's triangle count. <br>

## Gallery

512 Samples with a max path depth of 8 and no direct light sampling:
![512samples-8depth-0ds-demo4](https://user-images.githubusercontent.com/35866844/52524068-924af280-2c4d-11e9-8d52-190e330196fa.PNG)

512 Samples with a max path depth of 8 with 1 direct light sample:
![512samples-8depth-1ds-demo4](https://user-images.githubusercontent.com/35866844/52524071-9e36b480-2c4d-11e9-8fc4-04d53b6697da.PNG)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

