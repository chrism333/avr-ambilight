/* The MIT License (MIT)
 *
 * Copyright (c) 2015 Christian Menard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ambilight.hpp"

#include <xpcc/architecture/platform.hpp>
#include <xpcc/io/iostream.hpp>

#include <avr/wdt.h>



using namespace xpcc::atmega;
typedef xpcc::avr::SystemClock systemClock;

// Use the I2C device as a master with a baudrate of 100.000



//     static void setRgbLed(uint8_t led, uint8_t r, uint8_t g, uint8_t b)
//     {
//         if ( led < numRgbLeds)
//         {
//             uint8_t controllerId  = led / numControllerRgbLeds;
//             uint8_t controllerLed = led % numControllerRgbLeds;
//
//             uint16_t red   = static_cast<uint16_t>(r) << 4;
//             uint16_t green = static_cast<uint16_t>(g) << 4;
//             uint16_t blue  = static_cast<uint16_t>(b) << 4;
//
//             // because of the PCB design Leds need to be mapped to pwm channels
//             switch (controllerLed)
//             {
//             case 0:
//                 controller[controllerId].setChannel(2, red);
//                 controller[controllerId].setChannel(1, green);
//                 controller[controllerId].setChannel(0, blue);
//                 break;
//             case 1:
//                 controller[controllerId].setChannel(3, red);
//                 controller[controllerId].setChannel(4, green);
//                 controller[controllerId].setChannel(5, blue);
//                 break;
//             case 2:
//                 controller[controllerId].setChannel(9, red);
//                 controller[controllerId].setChannel(7, green);
//                 controller[controllerId].setChannel(6, blue);
//                 break;
//             case 3:
//                 controller[controllerId].setChannel(12, red);
//                 controller[controllerId].setChannel(11, green);
//                 controller[controllerId].setChannel(10, blue);
//                 break;
//             case 4:
//                 controller[controllerId].setChannel(13, red);
//                 controller[controllerId].setChannel(14, green);
//                 controller[controllerId].setChannel(15, blue);
//                 break;
//             default:
//                 break;
//             }
//         }
//     }
//
//     static void setRedLed(uint8_t led, uint16_t r)
//     {
//         if ( led < numRgbLeds)
//         {
//             uint8_t controllerId  = led / numControllerRgbLeds;
//             uint8_t controllerLed = led % numControllerRgbLeds;
//
//             // because of the PCB design Leds need to be mapped to pwm channels
//             switch (controllerLed)
//             {
//             case 0:
//                 controller[controllerId].setChannel(2, r);
//                 break;
//             case 1:
//                 controller[controllerId].setChannel(3, r);
//                 break;
//             case 2:
//                 controller[controllerId].setChannel(9, r);
//                 break;
//             case 3:
//                 controller[controllerId].setChannel(12, r);
//                 break;
//             case 4:
//                 controller[controllerId].setChannel(13, r);
//                 break;
//             default:
//                 break;
//             }
//         }
//     }
//
//     static void setGreenLed(uint8_t led, uint16_t g)
//     {
//         if ( led < numRgbLeds)
//         {
//             uint8_t controllerId  = led / numControllerRgbLeds;
//             uint8_t controllerLed = led % numControllerRgbLeds;
//
//             // because of the PCB design Leds need to be mapped to pwm channels
//             switch (controllerLed)
//             {
//             case 0:
//                 controller[controllerId].setChannel(1, g);
//                 break;
//             case 1:
//                 controller[controllerId].setChannel(4, g);
//                 break;
//             case 2:
//                 controller[controllerId].setChannel(7, g);
//                 break;
//             case 3:
//                 controller[controllerId].setChannel(11, g);
//                 break;
//             case 4:
//                 controller[controllerId].setChannel(14, g);
//                 break;
//             default:
//                 break;
//             }
//         }
//     }
//
//     static void setBlueLed(uint8_t led, uint16_t b)
//     {
//         if ( led < numRgbLeds)
//         {
//             uint8_t controllerId  = led / numControllerRgbLeds;
//             uint8_t controllerLed = led % numControllerRgbLeds;
//
//             // because of the PCB design Leds need to be mapped to pwm channels
//             switch (controllerLed)
//             {
//             case 0:
//                 controller[controllerId].setChannel(0, b);
//                 break;
//             case 1:
//                 controller[controllerId].setChannel(5, b);
//                 break;
//             case 2:
//                 controller[controllerId].setChannel(6, b);
//                 break;
//             case 3:
//                 controller[controllerId].setChannel(10, b);
//                 break;
//             case 4:
//                 controller[controllerId].setChannel(15, b);
//                 break;
//             default:
//                 break;
//             }
//         }
//     }


int main()
{
    // Create a new UART object and configure it to a baudrate of 38.400
    Uart0 uart;
    uart.initialize<systemClock, 38'400>();

    // Set-up the I2C device as master and configure it to a baudrate of 100.000
    I2cMaster::initialize<systemClock, 500'000>();

    // Create a IOStream for complex formatting tasks
    xpcc::IODeviceWrapper< Uart0, xpcc::IOBuffer::BlockIfFull > device;
    xpcc::IOStream stream(device);

    stream << "Hello World!" << xpcc::endl;

    // Enable interrupts
    sei();

    wdt_enable(WDTO_1S);
    wdt_reset();

    Ambilight ambilight;

    uint16_t x;

    while (true)
    {
        /*
         * For some reason the Watchdog on my device is always enabled (although
         * WDTON is not programmed). Therefore the Watchdog needs to be reset
         * regulary.
         */

        wdt_reset();

        ambilight.run();

        x++;
        if(x == 100)
        {
            x = 0;
            for(int i = 0; i < 30; i++)
            {
                uint16_t tmp = ambilight.leds[i].getBlue();
                if(tmp ==0xfff)
                    tmp = 0;
                else
                    tmp++;

                ambilight.leds[i].setBlue(tmp);
            }
        }

    }
}
