# IT'S ALIVE

I made a little animatronic because I wanted to believe that I could and now I did and it's done. I have made many mistakes. I feel triumphant.

## What is it?

I programmed a Raspberry Pi Pico to control a few LEDs and servos. I made an extraordinarily rudimentary animatronic, I programmed it in C, and I almost lost my mind.
Anything good about this bad boy I learned from [Harry Fairhead](https://iopress.info/index.php/author-s-pages/harry-fairhead). 

I used the Rasberry Pi Pico quickstart guide and stuck to it like this abominable superglue I have still on my hands (more on that later). [Chapter 8](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf?_gl=1*thq7f3*_ga*OTc0Mjc3MTU0LjE3MDk0MDM0MjU.*_ga_22FD70LWDS*MTcxMjI4NzQ2Ny4xLjEuMTcxMjI4NzUzMy4wLjAuMA..) is the real juice here. If you want to take this code (and really take Harry's code) and expand on it or build it or do anything, you can basically stick to the guide of getting started with C. I did this all on a Raspberry Pi 400 because it was way easier to compile and develop and use the serial connection to help me debug my code. So, if you set your whole machine up do develop in C like the docs tell you, you can clone this repo and do all that `cmake -j4` jazz to compile `dr-robo` into a `uf2` and flash it into a Pico. Not a Pico W, just a regular one with no wifi.  

### My little dude is named Unit 777-9311
He is a robot from beyond the furthest moons of jupiter whose robotic society, in 1985, were transfixed by Purple Rain when Prince broadcast the concert from Syracuse.
It escaped the atmosphere of earth and echoed through space and time. The robots' recievers rejoiced when they heard Prince. They knew it was imperative to send one of 
their own down to this planet, Earth, and transmit it's finest funk music back home before the robots invaded the planet and obliterated the human race.

## What does it do

Not much dude. It has too many buttons and they don't do enough. I did not really think this through and couldn't grapple with the complexities of things like reading from a
rotary encoder in C. I could barely get the SDK installed and compile a program let alone do that. Two servos are controlled by five buttons, an LED is controlled by the sixth.

- The servos have 180 degrees of motion. One servo is attached to one arm, the other servo is attached to the neck. Another way of saything this is one servo moves the arm up and down the other turns the head side to side. I also was totally intellectually wrecked trying to figure out mechanical engineering. Translating motion? No way my dude, not this time around. I studied Shakespeare not this kind of science, and I struggled with glueing plastic to sculpey, translating mechanical motion will come later. Maybe it will come with 3D printing. 
- One button resets the servos to their default position
- One button selects which servo you want to control, the arm or the head
- Two of the buttons control which way you move the current servo, one rotates clockwise and the other counter clockwise
- One of the buttons changes the type of motion the servo will take, either small incremental moves like the second hand on a clock or large swings to the minimum or maximum rotation
- One button resets the position of both servos and returns control to the default servo
- The last button controls the LED in the mouth, if you press the button the LED turns on, but with a sense of character you know? I used PWM to make the light ease in and out (thanks for that Harry)

## What went right

Man I learned so much it's nuts. I had no idea what compiling a program was before I started this. For work I write JavaScript and really man that's just playing pinball with the tilt turned off. Harry's book is really incredible. I like the concept of building a finite state machine. There's still so much I don't know but I think I'm starting to understand the scope of this little electric playground and just how much there is to learn.

## What I never want to do again because wow it feels bad man

- Don't build fussy controls quickly, it would've been worth the time to use some sort of encoder or knob to handle turning the servo, these buttons will make puppeteering awful
- Don't build fussy controls period, now I have a robot that's done but hard to puppet, I'll have to build it again
- Don't be an arrogant clown and instead learn something about 3D printing, cardboard and glue is sorta romantic but honestly man come on so is E V E R Y T H I N G to someone else
- Don't superglue things together if you're not wearing gloves and expect your hands to stay clean
- Don't start trying to reinvent the art of origami
- Don't try to reinvent everything man there's already a lot of pretty good stuff out there and it's cool to use it
- Don't romanticize doing things the hard way, but recognize the value
- Don't think you can write software drunk
- Don't think you can read a book drunk
- Don't forget to eat your vegetables while you're at it


## Final thoughts
Please do take care of each other, may u live 2 see the dawn