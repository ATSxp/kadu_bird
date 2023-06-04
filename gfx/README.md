# Graphics folder 
For the Game Boy Advance to recognize these images, they must be converted to text files (C, ASM, etc). 
Using [Grit](https://www.coranac.com/man/grit/html/grit.htm), let's transform our images into GBA readable files, so I don't need to convert image by 
image, line by line, Grit allows you to use a text file `.grit` that is used by the program to pass 
its parameters, following the steps on the site, you will be able to configure your `Makefile` to convert 
the graphics at compilation time.

To draw the sprites and backgrounds, I used [Grafx2](http://grafx2.chez.com/), because with 
it I could manipulate the palettes I was using.

*Grafx2 is very old school, i recommend =)*

## Converting a single file
Create a .grit file with the same name as the image you want to convert and write the parameters, as 
if it were on a command line:
```grit
# Single file
-gt -gB4 # etc...
```
*Remembering that the two files (grit and the image) must be in the same folder.* 

## Converting multiple files
If by chance you are converting several images with the same options, I suggest doing the 
following, create a folder (inside this one) and put all the desired images in it, after 
that, inside the folder, write your `grit`  file with the same name as the current folder. 
And don't forget to add it to the `Makefile` .
```makefile
# Makefile

# etc...
GRAPHICS	:= gfx \
            # some folder... \
            <your_folder>
# etc...
```
