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

#include <xpcc/architecture/platform.hpp>
#include <xpcc/io/iostream.hpp>

#include <avr/wdt.h>

using namespace xpcc::atmega;
typedef xpcc::avr::SystemClock systemClock;

int main()
{
    // Create a new UART object and configure it to a baudrate of 38400
    Uart0 uart;
    uart.initialize<systemClock, 38400>();

    // Enable interrupts, this is needed for every buffered UART
    sei();

    // Create a IOStream for complex formatting tasks
    xpcc::IODeviceWrapper< Uart0, xpcc::IOBuffer::BlockIfFull > device;
    xpcc::IOStream stream(device);

    // Now we can print numbers and other objects to the stream
    // The following will result in the string "24 is a nice number!\n" with
    // the number formatted as ASCII text.
    stream << "Hello World!" << xpcc::endl;

    while (true)
    {
        /*
         * For some reason the Watchdog on my device is always enabled (although
         * WDTON is not programmed). Therefore the Watchdogs needs to be reset
         * regulary.
         */

        wdt_reset();
    }
}
