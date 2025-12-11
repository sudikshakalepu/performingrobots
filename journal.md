# Judge's Robotic Journey

**Date:** 09/11/2025
Done:
* Attached wheel motors and castor wheel (with a few layers of cardboard between the main base and the wood piece to which the castor is bolted so that the robot plank is parallel to the floor). 
* Discussed and planned all the features we want in the robot:
    * Tall , feminine, light frame: head and body made out of an oval ball and corset made out of glue gun and cardboard
    * Cigar arm (servo at shoulder)
    * Cigar effect: (a) break into the circuit of an aroma diffuser to add to the smoke effect or (b) add a neopixel ring to animate cigar smoking 
    * Gavel arm (servo at shoulder)
    * Servo at the neck to make the head turn towards the cigar for human-like smoking scenes
Next up:
* Figure out diffuser circuit or other alternative for physical smoke.


**Date:** 09/25/2025
Done:
* Attached wheels, Arduino and Hobby RC shield for the wheels, H-bridge, tested it with basic motor code.
* Tested robot driving using the Hobby RC. Decided Aysha is the better driver and should drive.
* Scrapped diffuser idea, neopixel seems most plausible to not raise safety concerns in the Black Box on D-day. Might use a box (with a downward smoke incense in it) attached to a servo to open and close it to let smoke out whenever cigar is triggered.
Next up:
* Figure out what to use for the robot's skeleton and shoulder.

**Date:** 10/02/2025
Done:
* Attached a sturdy, thick, cardboard cylinder to use as the spine.
* Drilled rectangular slots for a wooden plank to be passed through the carboard spine as the shoulder. Attached the shoulder using nuts and bolts on either sides of the cardboard spine where the plank passes through so that it doesn't slide out.
* Built the corset and head using carboard and hot glue. 
* Attached a servo on the inside of the head using a lasercut ply wood circle. Decided we don't need a lazy susan because there isn't much weight to balance in the head, and the head was squishy and sphere-ish. Attached a plastic black wheel thing that we borrowed from Professor.
* Attached 2 servos on both ends of the shoulder plank to be used for cigar and gavel arms.
Next up:
* Figure out what to use for the robot's arms and if we need elbow servos.

<img src="/images/elbow.jpeg" alt="Elbow" style="width: 50%; height: auto; display: block; margin: 10px 0;">

**Date:** 10/23/2025
Done:
* Attached bamboo sticks for arms. Decided to add a pool noodle around to give the arms some flesh.
* Decided against elbow servos. Fastened (nut and bolt, and zipties) a bent lower arm on the cigar side to help Her hold the cigar.
* Screwed in the Arduino Mega and its shield onto the shoulder. Ensured all attached servos work using code.
Next up:
* Figure out radio transmission for the servos.
* Decide on the feature that made it obvious to the audience that the Judge was speaking or talking (possibly neopixel rings for eyes. potentially show her guilt, anger and flustered looks)

<img src="/images/servos.jpeg" alt="Servos" style="width: 50%; height: auto; display: block; margin: 10px 0;">


  
**Date:** 11/06/2025
Done:
* Unreliable radio transmission, would work sometimes. Replaced nRF module, worked.
* Coded all cases for the robot as per the script, multitasking implemented for only some cases.
* Generated audio files and uploaded to SD card.
* Attached Music Maker Shield but was unreliable in the beginning. After nRF module was replaced, fixed itself.
* Audio quality from the speaker was not satisfactory.
Next up:
* Figure out audio quality issue.
* Calibrate angles to do the right motion.
  
<img src="/images/lcd1.jpg" alt="LCD Display 1" style="width: 50%; height: auto; display: block; margin: 10px 0;">
<img src="/images/lcd2.jpg" alt="LCD Display 2" style="width: 50%; height: auto; display: block; margin: 10px 0;">

**Date:** 12/03/2025
Done:
* Calibrated angles for all servos.
* Adjusted all neopixel animations except cigar.
* Changed .wav files to .mp3, audio quality issues fixed.
* Added pool noodles to arms, made cigar and gavel using cardboard roll, thermocol cylinders, tape and pool noodle.
* Added castors to plank perpendicular to the slant edges of the robot base.
* Changed gavel servo horn to a metal one, after She met with an accident and fell during a rehearsal.
Next up:
* Dress her up.
* Make final edits to neopixel animation.

<img src="/images/works.jpeg" alt="Works" style="width: 50%; height: auto; display: block; margin: 10px 0;">

**Date:** 12/08/2025
Done:
* Added cases for just smoking and gavel bang for when the Judge has nothing to say and is listening.
* Added a function to move motors smoothly to emulate a calm and composed disposition of Judge as described by another performer.
* Transmitter error, last few cases wouldn't show up on the LCD. Fixed by increasing to number of states to existing+10.
* Painted her face, added wooden skewers through her head and into the little black wheel that the head servo was attached to and a few nuts suspended on a nylon thread tied and left behind her head to keep her face from tipping forward. Stuffed her face and corset with paper to give it some shape and form.
* Dressed her up in an _abaya_, pinned up her hair.
* Made final edits to neopixel animation, to almost sync the gavel bang audio to gavel bang motion.
* Decided to not add a servo for physical smoke.

<img src="/images/ready.jpeg" alt="Ready" style="width: 50%; height: auto; display: block; margin: 10px 0;">
<img src="/images/show.jpeg" alt="Show" style="width: 100%; height: auto; display: block; margin: 10px 0;">
<img src="/images/yay.jpeg" alt="Yay" style="width: 100%; height: auto; display: block; margin: 10px 0;">
