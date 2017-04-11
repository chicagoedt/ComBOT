from graphics import *
from random import randint
from time import *

timeRef = 0.05
def main():

    bgColor = color_rgb(0,0,0)
    eyeColor = color_rgb(100,225,200)
    ballColor = color_rgb(75,200,175)
    stateCheck = 0
    i = 0

    width = 800
    height = width*(5/3)
    root = GraphWin("Wrench2.0", width, height)
    root.setCoords(-width/2,-height/2,width/2,height/2)
    root.setBackground(bgColor)

    center  = Point(0,0)
    eyeSize = 50 * (width/480)
    eyeSeparate = 100  * (width/480)
    eyeHeight = 200 *(height/800)
    eyeWidth = 20
    # Basic eyebrows
    browSep = 100 *(height/800)
    browDent = 25 *(height/800)
    browSide1 = 50 *(height/800)
    browSide2 = 125 *(height/800)
    browTilt = 5 *(height/800)
    browOffset = 40  * (width/480)
    pointBrowL1= Point(-eyeSeparate - browSide1 - browOffset, eyeHeight + browSep - browTilt)
    pointBrowL2= Point(-eyeSeparate - browOffset, eyeHeight + browSep + browDent)
    pointBrowL3= Point(-eyeSeparate + browSide2 - browOffset, eyeHeight + browSep + browTilt)
    pointBrowR1= Point(eyeSeparate - browSide2 + browOffset, eyeHeight + browSep + browTilt)
    pointBrowR2= Point(eyeSeparate + browOffset, eyeHeight + browSep + browDent)
    pointBrowR3= Point(eyeSeparate + browSide1 + browOffset, eyeHeight + browSep - browTilt)
# Angry Eyebrows
    a_browSep = 100 *(height/800)
    a_browDent = 30 *(height/800)
    a_browSide1 = 50*(height/800)
    a_browSide2 = 125*(height/800)
    a_browTilt = -20*(height/800)
    a_browOffset = 25 * (width/480)
    a_pointBrowL1= Point(-eyeSeparate - a_browSide1 - a_browOffset, eyeHeight + a_browSep - a_browTilt)
    a_pointBrowL2= Point(-eyeSeparate - a_browOffset, eyeHeight + a_browSep + a_browDent)
    a_pointBrowL3= Point(-eyeSeparate + a_browSide2 - a_browOffset, eyeHeight + a_browSep + a_browTilt)
    a_pointBrowR1= Point(eyeSeparate - a_browSide2 + a_browOffset, eyeHeight + a_browSep + a_browTilt)
    a_pointBrowR2= Point(eyeSeparate + a_browOffset, eyeHeight + a_browSep + a_browDent)
    a_pointBrowR3= Point(eyeSeparate + a_browSide1 + a_browOffset, eyeHeight + a_browSep - a_browTilt)
# Sad Eyebrows
    s_browSep = 100*(height/800)
    s_browDent = 20*(height/800)
    s_browSide1 = 50*(height/800)
    s_browSide2 = 125*(height/800)
    s_browTilt = 30*(height/800)
    s_browOffset = 50  * (width/480)
    s_pointBrowL1= Point(-eyeSeparate - s_browSide1 - s_browOffset, eyeHeight + s_browSep - s_browTilt)
    s_pointBrowL2= Point(-eyeSeparate - s_browOffset, eyeHeight + s_browSep + s_browDent)
    s_pointBrowL3= Point(-eyeSeparate + s_browSide2 - s_browOffset, eyeHeight + s_browSep + s_browTilt)
    s_pointBrowR1= Point(eyeSeparate - s_browSide2 + s_browOffset, eyeHeight + s_browSep + s_browTilt)
    s_pointBrowR2= Point(eyeSeparate + s_browOffset, eyeHeight + s_browSep + s_browDent)
    s_pointBrowR3= Point(eyeSeparate + s_browSide1 + s_browOffset, eyeHeight + s_browSep - s_browTilt)

    initBallOffset = -5 * (width/480)
    initBallSize = 15 * (width/480)
    flipFlop = 0
    tempX = 0
    tempY = 0

    pointEyeL = Point(-eyeSeparate,eyeHeight)
    pointEyeR = Point(eyeSeparate,eyeHeight)

    pointBallL = Point(-eyeSeparate,eyeHeight + initBallOffset )
    pointBallR = Point(eyeSeparate,eyeHeight + initBallOffset)

    eyeL = Circle(pointEyeL,eyeSize)
    eyeL_blink = Line(Point(-eyeSeparate-(eyeSize),eyeHeight),Point(-eyeSeparate+(eyeSize),eyeHeight))
    eyeL_blink.setOutline(eyeColor)
    eyeL_blink.setWidth(eyeWidth + 10)
    eyeL.setWidth(eyeWidth)
    eyeL.setOutline(eyeColor)

    eyeR = Circle(pointEyeR,eyeSize)
    eyeR_blink = Line(Point(eyeSeparate-(eyeSize),eyeHeight),Point(eyeSeparate+(eyeSize),eyeHeight))
    eyeR_blink.setOutline(eyeColor)
    eyeR_blink.setWidth(eyeWidth + 10)
    eyeR.setWidth(eyeWidth)
    eyeR.setOutline(eyeColor)

    eBL = Circle(pointBallL,initBallSize)
    eBR = Circle(pointBallR,initBallSize)
    eBL.setFill(ballColor)
    eBR.setFill(ballColor)
    eBL.setWidth(0)
    eBR.setWidth(0)

    browL = Polygon(pointBrowL1,pointBrowL2,pointBrowL3)
    browR = Polygon(pointBrowR1,pointBrowR2,pointBrowR3)
    browL.setWidth(0)
    browL.setFill(eyeColor)
    browR.setWidth(0)
    browR.setFill(eyeColor)

    a_browL = Polygon(a_pointBrowL1,a_pointBrowL2,a_pointBrowL3)
    a_browR = Polygon(a_pointBrowR1,a_pointBrowR2,a_pointBrowR3)
    a_browL.setWidth(0)
    a_browL.setFill(eyeColor)
    a_browR.setWidth(0)
    a_browR.setFill(eyeColor)

    s_browL = Polygon(s_pointBrowL1,s_pointBrowL2,s_pointBrowL3)
    s_browR = Polygon(s_pointBrowR1,s_pointBrowR2,s_pointBrowR3)
    s_browL.setWidth(0)
    s_browL.setFill(eyeColor)
    s_browR.setWidth(0)
    s_browR.setFill(eyeColor)

    ####################
    browL.draw(root)
    browR.draw(root)
    eyeL.draw(root)
    eyeR.draw(root)
    eBL.draw(root)
    eBR.draw(root)
    ####################
    while(1):

        key = root.checkKey()
        print(key)
        if(key == "space"):
            s_browR.undraw()
            s_browL.undraw()
            browR.undraw()
            browL.undraw()
            anger(root,eyeL,eyeR,eBL,eBR,a_browL,a_browR)
            stateCheck = 1
            sleep(0.01)
        elif(key == "Return"):
            a_browR.undraw()
            a_browL.undraw()
            browR.undraw()
            browL.undraw()
            sad(root,eyeL,eyeR,eBL,eBR,s_browL,s_browR)
            stateCheck = 2
            sleep(0.01)
        elif((key == "") and (stateCheck == 0)):
            a_browR.undraw()
            a_browL.undraw()
            s_browR.undraw()
            s_browL.undraw()


            eyeL.setOutline(eyeColor)
            eyeR.setOutline(eyeColor)
            browL.setFill(eyeColor)
            browR.setFill(eyeColor)
            eBL.setFill(ballColor)
            eBR.setFill(ballColor)
            idler(root,eyeL,eyeR,eyeL_blink,eyeR_blink,eBL,eBR)
            tempX, tempY, flipFlop = brownian(eBL,eBR,tempX,tempY,flipFlop)
        if((stateCheck == 1) or (stateCheck == 2)) :
            i = i + 1
            sleep(0.1)
            print("SleepState Reached")
        if(i == 20):
            print("Restart Reached")
            stateCheck = 0
            browL.draw(root)
            browR.draw(root)
        if(stateCheck == 0):
            i = 0


def idler(canvas,eL,eR,eL_blink,eR_blink,ebL,ebR):# State when nothing is happening
    sleep(timeRef) # DO NOT REMOVE -> Causes blinking real fast
    blinker = randint(0,50) + randint(0,50)
    #print("Blinker = " + str(blinker))
    if (blinker == 41):
        eL.undraw()
        eR.undraw()
        ebL.undraw()
        ebR.undraw()
        blink(canvas,eL,eR,eL_blink,eR_blink,ebL,ebR)
        ebL.draw(canvas)
        ebR.draw(canvas)
        eL.draw(canvas)
        eR.draw(canvas)
def blink(canvas,eL,eR,blink_objL,blink_objR,ebL,ebR): #Makes the eyes blink
    nat_blink = randint(0,2)

    blinkDelay = 0.3
    #print("natBlink = " + str(nat_blink))
    if (nat_blink == 1):
        blinkDelay = 0.1
        blink_objL.draw(canvas)
        blink_objR.draw(canvas)
        sleep(blinkDelay)
        blink_objL.undraw()
        blink_objR.undraw()
        eL.draw(canvas)
        eR.draw(canvas)
        ebL.draw(canvas)
        ebR.draw(canvas)
        sleep(blinkDelay)
        eL.undraw()
        eR.undraw()
        ebL.undraw()
        ebR.undraw()
    blink_objL.draw(canvas)
    blink_objR.draw(canvas)
    sleep(blinkDelay)
    blink_objL.undraw()
    blink_objR.undraw()
def brownian(bL,bR,tempX,tempY,flipFlop): # Moves the eye ball randomly
    clicker = randint(0,50)
    print(clicker)
    limit = 15
    if(clicker == 5):
        flickerX = randint(-limit,limit)
        flickerY = randint(-limit,limit)
        print("Moving Reached")

        if (flipFlop == 0):
            bL.move(flickerX,flickerY)
            bR.move(flickerX,flickerY)
            tempX = flickerX
            tempY = flickerY
            flipFlop = 1
            print("Working")
        else:
            bL.move(-tempX,-tempY)
            bR.move(-tempX,-tempY)
            flipFlop = 0

    return tempX,tempY,flipFlop
def anger(canvas,eL,eR,eBL,eBR,bL,bR):
    Angry = color_rgb(200,75,75)
    eL.undraw()
    eR.undraw()
    eBL.undraw()
    eBR.undraw()
    bL.undraw()
    bR.undraw()
    eL.setOutline(Angry)
    eR.setOutline(Angry)
    eBL.setFill(Angry)
    eBR.setFill(Angry)
    bL.setFill(Angry)
    bR.setFill(Angry)
    eL.draw(canvas)
    eR.draw(canvas)
    eBL.draw(canvas)
    eBR.draw(canvas)
    bL.draw(canvas)
    bR.draw(canvas)
    return
def sad(canvas,eL,eR,eBL,eBR,bL,bR):
    Sadness = color_rgb(75,75,200)
    eL.undraw()
    eR.undraw()
    eBL.undraw()
    eBR.undraw()
    bL.undraw()
    bR.undraw()
    eL.setOutline(Sadness)
    eR.setOutline(Sadness)
    eBL.setFill(Sadness)
    eBR.setFill(Sadness)
    bL.setFill(Sadness)
    bR.setFill(Sadness)
    eL.draw(canvas)
    eR.draw(canvas)
    eBL.draw(canvas)
    eBR.draw(canvas)
    bL.draw(canvas)
    bR.draw(canvas)
    return


###################################
#######Calling function Here#######
main()
