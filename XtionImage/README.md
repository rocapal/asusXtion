# XtionImage


XtionImage is a tool to get images (color, depth and IR) from Asus Xtion sensors using openni2 library. You can get images of both resolutions (320x240 and 640x480). It's tested in the following architectures: 386, amd64, armhf (raspberrypi).


## Requisites


It's needed openni2. I recommend you install openni2 using package debian that you can get in: 

* i386: http://jderobot.org/apt/dists/debian/unstable/binary-amd64/openni2_2.2.0-1jderobot_amd64.deb
* amd64: http://jderobot.org/apt/dists/debian/unstable/binary-i386/openni2_2.2.0-1jderobot_i386.deb
* armhf: http://github.com/rocapal/asusXtion/blob/master/debian/openni2_2.2.0-1jderobot_armhf.deb

Also, you can get openni2 from its GIT repository and install under /usr/lib/openni2: https://github.com/OpenNI/OpenNI2


## Compilation

* First, clone the git repository
`git clone git://github.com/rocapal/asusXtion.git`

* Run cmake and make
```bash
$ cd asusXtion
$ cd XtionImage
$ cmake .
$ make
```
A binary called XtionImage has been generated


## Usage

```bash
$ ./XtionImage 
Usage:
	 -r : Resolution of image
		 0: for 320x240
		 1: for 640x480

	 -s : Sensor Type
		 c: for color image
		 d: for depth image
		 i: for ir image

	 -o : file name path

	 Example: XtionImage -r 0 -s d -o /tmp/image.jpg
```

## Examples

* Get color image to 320x240
```bash
$ ./XtionImage -r 0 -s c -o /tmp/image.jpg
```
![ScreenShot](https://github.com/rocapal/asusXtion/raw/master/images/color-image.png)

* Get depth image to 320x240
```bash
$ ./XtionImage -r 0 -s d -o /tmp/image.jpg
```
![ScreenShot](https://github.com/rocapal/asusXtion/raw/master/images/depth-image.png)

* Get IR image to 640x480
```bash
$ ./XtionImage -r 1 -s i -o /tmp/image.jpg
```
![ScreenShot](https://github.com/rocapal/asusXtion/raw/master/images/ir-image.png)

## License

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/.
