# pl_parallel

Parallel driver for PL Germany devices.

# How To

## Communication with PLG devices

The communication is realized via a character device created by the driver. This character device can be found at '/dev/parallel'.

Writing into this file will trigger a send event on the parallel bus.
This will include both, a command send event at the beginning continued by a data send event.
The first 16-bit of the file input stream will be send as a address/command. All other 16-bit data will be counted as data.

In the following example the command 0x05CF will be send continued by the data 0x2020 and 0x4FFC:

```sh
user@beaglebone:~$ echo 05CF20204FFC | xxd -p -r > /dev/parallel
user@beaglebone:~$
```

The next example shows how to send just a address/command to the device. Here, the command 0x0302 will be send:

```sh
user@beaglebone:~$ echo 0302 | xxd -p -r > /dev/parallel
user@beaglebone:~$
```

Reading from the device is equally simple. A file read will trigger a read event on the parallel bus.

In this example a address/command will be send to the device followed by a read event.
The device gets the command 0x4FFC and will then answer with 4 bytes of data.

```sh
user@beaglebone:~$ echo 4FFC | xxd -p -r > /dev/parallel ; head -c 4 /dev/parallel | xxd
00000000:  4441 5440
user@beaglebone:~$
```

## Parallel bus configuration

The driver provides an interface for the user to change various timings and signal polarities.

This interface can be found in the 'pl_par' class:

```sh
user@beaglebone:~$ ls /sys/class/pl_par
burst_en parallel polarities timings
user@beaglebone:~$ 
```

### General settings

The general parallel bus settings can be found inside the main folder 'pl_par'.

```sh
user@beaglebone:~$ ls /sys/class/pl_par
burst_en parallel polarities timings
user@beaglebone:~$ 
```

burst_en [1,0]

> Enables/Disables the burst write mode. In this mode the HRDY signal is ignored and the data is send as fast as possible.
> 1 enables the mode, 0 disable it.

### Timing settings

All timings are includes in the timings folder:

```sh
user@beaglebone:~$ ls /sys/class/pl_par/timings
clk_div  clk_freq  cs_delay  power  r_hold  r_strobe  r_su  subsystem  uevent  w_hold  w_strobe  w_su
user@beaglebone:~$ 
```

clk_div [integer]

> Module clock divider.
> This defines the divider for the input clock.

clk_freq [integer]

> This defines the modules input frequency [Hz]. This frequency divided by the 
> clock divider results into the buss master clock frequency.

cs_delay [integer]

> This defines the number of memclk cycles between the end of one device access 
> and the start of another device access unless the two accesses are both Reads.
> In this case, this delay is not incurred.

r_hold [integer]

> Read Strobe Hold cycles.
> This defines the number of memclk cycles for which Data Bus/Pad Output Enable, 
> the Direction bit, and Chip Select are held after the Read Strobe is 
> deasserted when performing a read access.

r_strobe [integer]

> Read Strobe Duration cycles.
> This defines the number of memclk cycles for which the Read
> Strobe is held active when performing a read access.

r_su [integer]

> Read Strobe Set-Up cycles.
> When performing a read access, this defines the number of
> memclk cycles that Data Bus/Pad Output Enable, the Direction bit,
> and Chip Select have to be ready before the Read Strobe is
> asserted.

w_hold [integer]

> Write Strobe Hold cycles.
> This defines the number of memclk cycles for which Data
> Bus/Pas Output Enable, ALE, the Direction bit, and Chip Select 0
> are held after the Write Strobe is de-asserted when performing write
> access.

w_strobe [integer]

> Write Strobe Duration cycles.
> This defines the number of memclk cycles for which the Write
> Strobe is held active when performing a write access.

w_su [integer]

> Write Strobe Set-Up cycles.
> When performing a write access, this defines the number of
> memclk cycles that Data Bus/Pad Output Enable, the Direction bit,
> and Chip Select have to be ready before the Write Strobe is
> asserted.

### Polarity settings

All polarity settings can be found in the 'polarities' subfolder:

```sh
user@beaglebone:~$ ls /sys/class/pl_par/polarities
ale_pol  cs0_e0_pol  cs1_e1_pol  power  rs_en_pol  subsystem  uevent  ws_dir_pol
user@beaglebone:~$
```

ale_pol [0,1]

> Address Latch Enable (ALE) Polarity Control.
> 0 = Do Not Invert ALE.
> 1 = Invert.

cs0_e0_pol [0,1]

> Chip Select 0/Enable 0 (Secondary) Polarity Control.
> 0 = Do Not Invert Chip Select 0/Enable 0.
> 1 = Invert Chip Select 0/Enable 0.

cs1_e1_pol [0,1]

> Chip Select 1/Enable 1 (Secondary) Polarity Control.
> 0 = Do Not Invert Chip Select 1/Enable 1.
> 1 = Invert Chip Select 1/Enable 1.

rs_en_pol [0,1]

> Read Strobe/Direction Polarity Control.
> 0 = Do Not Invert Read Strobe/Direction.
> 1 = Invert Read Strobe/Direction.

ws_dir_pol [0,1]

> Write Strobe/Direction Polarity Control.
> 0 = Do Not Invert Write Strobe/Direction.
> 1 = Invert Write Strobe/Direction.