
---------------------------------------------------------------------------------------
**List of pre-installed nodes (<u>in development</u>) ** 

[TOC]
---------------------------------------------------------------------------------------
| ID | Name | Comments |
| ---- | -- | -- |
| 1 |arm_stream_script           |byte-code interpreter index "arm_stream_script_INDEX"                   |
| 2    | arm_stream_format_converter | raw data and frame format converter                          |
| 3 |arm_stream_router           |copy input arcs and subchannel and output arcs and subchannels          |
| 4 |sigp_stream_converter       |raw data interleaving sampling-rate converter                           |
| 5 |arm_stream_amplifier        |amplifier mute and un-mute with ramp and delay control                  |
| 6 |arm_stream_mixer            |multichannel mixer with mute/unmute and ramp control                    |
| 7 |arm_stream_filter           |cascade of DF1 filters                                                  |
| 8 |sigp_stream_detector        |estimates peaks/floor of the mono input and triggers a flag on high SNR |
| 9 |arm_stream_rescaler         |raw data values remapping using "interp1"                               |
|10 |sigp_stream_compressor      |raw data compression with adaptive prediction                           |
|11 |sigp_stream_decompressor    |raw data decompression                                                  |
|12 |arm_stream_modulator        |signal generator with modulation                                        |
|13 |arm_stream_demodulator      |signal demodulator frequency estimator                                  |
|14 |sigp_stream_resampler       |asynchronous high-quality sample-rate converter                         |
|15 |arm_stream_qos              |raw data interpolator with synchronization to one HQoS stream           |
|16 |arm_stream_split            |let a buffer be used by several nodes                                   |
|17 |sigp_stream_detector2D      |activity detection pattern detection                                    |
|18 |arm_stream_filter2D         |filter rescale/zoom/extract rotate exposure compensation                |
|19 |arm_stream_analysis         |arm_stream_analysis                                                     |
|20 |bitbank_jpg_encoder         |jpeg encoder                                                    |
|21 |elm_jpg_decoder                 |TjpgDec                                                                 |
|      |                             |                                                              |
|      |                             |                                                              |


# arm_stream_script

Scripts are nodes interpreted from byte codes, indexed in the SCRIPTS section of the graph. The data RAM is placed in a shared arc for all script (instance registers+stack parameters) constants are placed after the byte-codes.

The default memory configuration is "shared" meaning the buffers associated with the script are sharing the same memory buffer. 

To have individual static memory associated to a script the "script_mem_shared" must be 0.

Special functions activated with callsys and conditional instructions:
      - lock   : a block of nodes to a processor to have good cache performance, 
            - if-then: a block of nodes based on script decision (FIFO content/debug registers, ..)
   - loop   : repeat a list of node several time for cache efficiency and small frame size
   - Checks if the data it needs is available and returns to the scheduler

```
node arm_stream_script 1  ; script (instance) index           
    script_stack      12  ; size of the stack in word64      
    script_mem_shared  1  ; private memory (0) or shared(1)  
    script_mem_map     0  ; mapping to VID #0 (default)      

    script_code  
    
;   Assembler                            comments 
;   ---------                          ----------------
;   "test" operations 
;     testleq r6 sp                      test r6 <= top of the stack
;     testgt  r2 max { r4 #float 3.14}	 test r2 > max(r4, 3.14)
;
;   "load"
;     r6 sp                      r6 := top of the stack
;     r2 mul { r4 #float 3.14}	 r2 := (r4 x 3.14)
;
;   "mov"
;     scatter r2 r3 r4           r2[r4] = r3    indirect, write with indexes 
;     gather  r2 r3 #k           r2 = r3[k]     indirect, read with indexes 
;     wr2bf   r2 r3 LEN.POS      r2(bitfield) = r3 
;
;   "jump"
;     if_yes jump L_loop2, r2      jump to label loop2 and push r2 if test=yes
;     if_not callsys 15            system calls (fifo, time, setparam, io/hw)

;   "system call" use-cases :
;   - Node Set/ReadParam/Run, patch a field in the header
;   - FIFO read/set, data amount/free, time-stamp of last access 
;   - Update Format framesize, FS
;   - Read Time 16/32/64 from arc/AL, compute differences,
;   - Jump to +/- N nodes in the list
;   - Un/Lock a section of the graph
;   - Select new IO settings (set timer period, change gains)
;   - AL decides deep sleep with the help of parameters (wake-up in X[s])
;   - Trace "string" + data
;   - Callbacks (cmd (cmd+LIbName+Subfunction , X, n) for I2C/Radio .. Libraries 
;   - Callback returning the current use-case to set parameters
;   - Call a relocatable binary section in the Param area
;   - Share Physical HW addresses
;   - Compute Median from data in a circular buffer + mean/STD    
	end                                                      
	node_parameters <ID2>                                    
   	include 1 binary_code.txt ; path ID and file name     
	end   
```

----------------------------------------------------------------------------------------
# arm_stream_format_converter (TBD)

```
node arm_stream_format_converter 0

TBD

end
```



# arm_stream_router (TBD)

I/O streams have the same RAW format size, if time-stamped then only with frame size of 1 sample

table of routing : a list of 16bits words (source 8b + destination 8b)
format: router arc index (3b)           up to 8 arcs
        sub-channel index -1 (5b)       up to 32 channels

example with the router_2x2 used to merge 2 stereo input in a 4-channels output

```
node arm_stream_router 0
    parameters     0             ; TAG   "load all parameters"
        
        2  i8; 2 2          nb input/output arcs
        4 i16; 0 0 2 0      move arc0,chan0, to arc2,chan0
        4 i16; 0 1 2 1      move arc0,chan1, to arc2,chan1
        4 i16; 1 0 2 2      move arc1,chan0, to arc2,chan2
        4 i16; 1 1 2 3      move arc1,chan1, to arc2,chan3
    end
end
```

----------------------------------------------------------------------------------------
# sigp_stream_converter (TBD)

Operation : convert input arc format to the format of the output arc. 
Conversion takes care of RAW data, frame size, interleaving, time-stamp format, number of channels, sampling-rate.

Operations :
- copy input data to the first internal scratch buffer with the management of the target number of channels and using deinterleave intermediate format. The start of the deinterleaved buffer is a copy of the previously processed end of buffers, for the implementation of filters using a memory.
- sampling rate converter, with a dedicated processing for the asynchronous to synchronous conversion, to second scratch buffer
- raw data conversion, interleaving conversion and copy of the result

The case of IMU format is managed like other one-dimension format. 

The case of 2D format is special with operations of pixel area extraction, zoom, interpolate, rotation  and pixel format conversions.



Parameters :

```
node arm_stream_converter 0

TBD

end
```

---------------------------------------------------------------------------------------
# arm_stream_amplifier (TBD)


Operation : control of the amplitude of the input stream with controlled time of ramp-up/ramp-down. 
The gain control “mute” is used to store the current gain setting, being reloaded with the command “unmute”
Option : either the same gain/controls for all channels or list of parameters for each channel

Parameters :  new gain/mute/unmute, ramp-up/down slope, delay before starting the slope. 
Use-cases :
    Features : adaptive gain control (compressor, expander, AGC) under a script control with energy polling 
    Metadata features : "saturation occured" "energy"
    Mixed-Signal glitches : remove the first seconds of an IR sensor until it was self-calibrated (same for audio Class-D)


parameters of amplifier (variable size): 
TAG_CMD = 1, uint8_t, 1st-order shifter slope time (as stream_mixer, 0..75k samples)
TAG_CMD = 2, uint16_t, desired gain FP_8m4e, 0dB=0x0805
TAG_CMD = 3, uint8_t, set/reset mute state
TAG_CMD = 4, uint16_t, delay before applying unmute, in samples
TAG_CMD = 5, uint16_t, delay before applying mute, in samples

```
node  arm_stream_amplifier 0


    parameters     0             ; TAG   "load all parameters"
        1  i8;  1           load only rising/falling coefficient slope
        1 h16;  805         gain -100dB .. +36dB (+/- 1%)
        1  i8;  0           muted state
        2 i16;  0 0         delay-up/down
    end  
end
```

----------------------------------------------------------------------------------------
# arm_stream_mixer (TBD)

Operation : receives several mono or multichannel streams and produces one output arc. 
Mixer manages up to 8 multichannels input arcs, the number is given at STREAM_RESET stage.

Parameters : input arcs names, their associated gain and time-constant in [ms], name of the 
  output arc and an extra mixing gain
  + index of the HQoS input arc to use for the frame size to use, if the other arcs
    have flow-errors it will be managed with their respective OVERFLRD_ARCW2 index

 Synchronization operation : arranges multiple input streams are delivered at the same time.
    Use-case: multi-channel audio stream, each channel is processed with independent
     signal processing feature. The problem is to avoid (on MP devices) one channel to
     be delivered to the final mixer ahead and desynchronized from the others
    (should be OK when the arcs have the sma buffer size / framesize)

preset : 
#1 (default) : mixer-4 with all arcs unmuted with gain = 0dB, slopes of ~1000 samples
#2 (shut-down) : mixer-4 with all arcs with gain = -96dB, slopes of ~100 samples
>2 : number of input channels

parameters of mixer (variable size): 
- slopes of rising and falling gains, identical to all channels
  slope coefficient = 0..15 (iir_coef = 1-1/2^coef = 0 .. 0.99)
  Convergence time to 90% of the target in samples:
   slope   nb of samples to converge
       0           0
       1           3
       2           8
       3          17
       4          36
       5          73
       6         146
       7         294
       8         588
       9        1178
      10        2357
      11        4715
      12        9430
      13       18862
      14       37724
      15       75450
      convergence in samples = abs(round(1./abs(log10(1-1./2.^[0:15])'))

- output mixer gain format FP

format to be packed in 32bits words: 
        input arc index (4b)        up to 15 arcs
        sub-channel index -1 (5b)   up to 32 channels per arc of the input arc
        sub-channel index -1 (5b)   up to 32 channels per arc of the output arc
        gain  (16b)                 format FP
        muted (1b)                  1 = "muted"

```
node arm_stream_mixer 0

;   Example: 4 input one mono, one stereo, output is stereo
    parameters     0             ; TAG   "load all parameters"
        

        2  i8; 6 6              rising/falling slope on all arcs
        1 h32; 807              -12dB output gain in FP_8m4e format; 

        4 i16; 0 0 0            arcID0 inputChan0 outputChan0 
        1 h32; 805              0dB gain; mono mixed on the left output

        4 i16; 0 0 1            arcID0 inputChan0 outputChan1 
        1 h32; 805              0dB gain; mono mixed on the right output
                   
        4 i16; 1 0 0            arcID1 inputChan0 outputChan0 
        1 h32; 805              0dB gain; stereo input left to the left

        4 i16; 1 1 1            arcID1 inputChan0 outputChan1 
        1 h32; 803             +12dB gain; stereo input right to the right
    end
end            
```


----------------------------------------------------------------------------------------
# arm_stream_filter
Operation : receives one multichannel stream and produces one filtered multichannel stream. 
Parameters : biquad filters coefficients used in cascade. Implementation is 2 Biquads max.
(see www.w3.org/TR/audio-eq-cookbook)
Presets:
#1 : bypass
#2 : LPF fc=fs/4 
#3 : HPF fc=fs/8 
#4 : DC-filter (use-case: audio, XYZ gravity compensation/estimation)
#5 : long Median filter

parameter of filter : 
- number of biquads in cascade (1 or 2)

```
node arm_stream_filter 0         node subroutine name + instance ID

    node_preset         1               ; parameter preset used at boot time, default = #0
    node_map_hwblock    0  0            ; list of "nb_mem_block" VID indexes of "procmap_manifest_xxxx.txt" where to map the allocated memory
                                        ;  default = #0
    node_map_verbose    1               ; level #1 of debug trace

    parameters     0             ; TAG   "load all parameters"
        1  u8;  2       Two biquads
        1  i8;  0       postShift
        5 f32; 0.284277f 0.455582f 0.284277f 0.780535f -0.340176f  b0/b1/b2/-a1/-a2  ellip(4, 1, 40, 3600/8000, 'low')
        5 f32; 0.284277f 0.175059f 0.284277f 0.284669f -0.811514f 
        ; or  _include    1   arm_stream_filter_parameters_x.txt      (path + file-name)
    end
end
```
----------------------------------------------------------------------------------------
# sigp_stream_detector

Operation : provides a boolean output stream from the detection of a rising 
edge above a tunable signal to noise ratio. 
A tunable delay allows to maintain the boolean value for a minimum amount of time 
Use-case example 1: debouncing analog input and LED / user-interface.
Use-case example 2: IMU and voice activity detection (VAD)
Parameters : time-constant to gate the output, sensitivity of the use-case

presets control
#1 : no HPF pre-filtering, fast and high sensitivity detection (button debouncing)
#2 : VAD with HPF pre-filtering, time constants tuned for ~10kHz
#3 : VAD with HPF pre-filtering, time constants tuned for ~44.1kHz
#4 : IMU detector : HPF, slow reaction time constants
#5 : IMU detector : HPF, fast reaction time constants

Metadata information can be extracted with the command "TAG_CMD" from parameter-read:
0 read the floor noise level
1 read the current signal peak
2 read the signal to noise ratio

```
node arm_stream_detector 0               node name  + instance ID

    preset              1               parameter preset used at boot time, default = #0
   
end
```


----------------------------------------------------------------------------------------
# arm_stream_rescaler (TBD)

 Operation : applies vq = interp1(x,v,xq) 
 Following https://fr.mathworks.com/help/matlab/ref/interp1.html
   linear of polynomial interpolation (implementation)
 Parameters : X,V vectors, size max = 32 points

no preset ('0')

```
node arm_stream_rescaler 0

    parameters     0             ; TAG   "load all parameters"
        
;               input   output
        2; f32; -1      1
        2; f32;  0      0       ; this table creates the abs(x) conversion
        2; f32;  1      1
    end  
end
```

---------------------------------------------------------------------------------------
# sigp_stream_compressor (TBD)

Operation : wave compression using IMADPCM(4bits/sample)
Parameters : coding scheme 

presets (provision codes):
- 1 : coder IMADPCM
- 2 : coder LPC
- 3 : 
- 4 : coder CVSD for BT speech 
- 5 : coder LC3 
- 6 : coder SBC
- 7 : coder mSBC
- 7 : coder OPUS Silk
- 8 : coder MP3
- 9 : coder MPEG-4 aacPlus v2 
- 10: coder OPUS CELT
- 11: coder JPEG

```
node 
    arm_stream_compressor 0

    parameters     0             ; TAG   "load all parameters"
        4; i32; 0 0 0 0     provision for extra parameters in other codecs
    end
end
```

----------------------------------------------------------------------------------------
# sigp_stream_decompressor (TBD)

Operation : decompression of encoded data
Parameters : coding scheme and a block of 16 parameter bytes for codecs

​	dynamic parameters : pause, stop, fast-forward x2 and x4.

    WARNING : if the output format can change (mono/stereo, sampling-rate, ..)
        the variation is detected by the node and reported to the scheduler with 
        "STREAM_SERVICE_INTERNAL_FORMAT_UPDATE", the "uint32_t *all_formats" must be 
        mapped in a RAM for dynamic updates with "COPY_CONF_GRAPH0_COPY_ALL_IN_RAM"
    
    Example of data to share with the application
        outputFormat: AndroidOutputFormat.MPEG_4,
        audioEncoder: AndroidAudioEncoder.AAC,
        sampleRate: 44100,
        numberOfChannels: 2,
        bitRate: 128000,

presets provision
- 1 : decoder IMADPCM
- 2 : decoder LPC
- 3 : MIDI player / tone sequencer
- 4 : decoder CVSD for BT speech 
- 5 : decoder LC3 
- 6 : decoder SBC
- 7 : decoder mSBC
- 7 : decoder OPUS Silk
- 8 : decoder MP3
- 9 : decoder MPEG-4 aacPlus v2 
- 10: decoder OPUS CELT
- 11: decoder JPEG 

```
node arm_stream_decompressor 0

    parameters     0             ; TAG   "load all parameters"
        4; i32; 0 0 0 0     provision for extra parameters in other codecs
    end
end
```

----------------------------------------------------------------------------------------
# arm_stream_modulator (TBD)


 Operation : sine, noise, square, saw tooth with amplitude or frequency modulation
 use-case : ring modulator, sweep generation with a cascade of a ramp generator and
    a frequency modulator

Parameters types and Tags:
 u8 #1 wave type : 1=sine 2=square 3=white noise 4=pink noise 
               5=sawtooth 6=triangle 7=pulse
               8=prerecorded pattern playback from arc 
               9=sigma-delta with OSR control for audio on PWM ports and 8b DAC
               10=PWM 11=ramp 12=step

parameter format for wave generation: 
```
u16 #2 linear amplitude, format UQ16 0dB .. -96dB
u16 #3 amplitude offset, format Q15 [-1 .. +1]
f32 #4 wave frequency [Hz] [0.1uHz .. 250kHz], 0Hz translates to DC level.
s16 #5 starting phase, format S15 [-pi .. +pi]/pi
u8  #6 modulation type, 0:amplitude, 1:frequency (reserved 2:FSK, ..)
u8  #7 modulation, 0:none 1=from arc bit stream, 2=from arc q15 words
u16 #8 modulation index (a) and offset (b), in q15 as fraction of frequency or amplitude
       modulation y=ax+b, x=input data.
f32 #9 modulation frequency [Hz] separating two data bits/samples from the arc
```

```
node arm_stream_modulator (i)

    parameters     0             ; TAG   "load all parameters"
        
        1  u8;  1       sinewave
        2 h16;  FFFF 0  full-scale, no offset
        1 f32;  1200    1200Hz
        1 s16;  0       initial phase
        2  u8;  1 1     frequency modulation from bit-stream
        2 h16;  8000 0  full amplitude modulation with sign inversion of the bit-stream
        1 f32;  300     300Hz modulation => (900Hz .. 1500Hz modulation)
    end
end
```

----------------------------------------------------------------------------------------
# arm_stream_demodulator (TBD)

 Operation : decode a bit-stream from analog data. Use-case: IR decoder, CAN/UART on SPI/I2S audio.
 Parameters : clock and parity setting or let the algorithm discover the frame setting after some time. https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter

presets control :
#1 .. 10: provision for  demodulators

Metadata information can be extracted with the command "parameter-read":
TAG_CMD = 1 read the signal amplitude
TAG_CMD = 2 read the signal to noise ratio

```
node 
    arm_stream_demodulator (i)
    parameters     0             ; TAG   "load all parameters"
        
        2  i8; 2 2          nb input/output arcs
        4 i16; 0 0 2 0      move arc0,chan0, to arc2,chan0
    end
end
```


----------------------------------------------------------------------------------------
# sigp_stream_resampler (TBD)

Operation : convert multichannel input data rate to the rate of the output arcs 
  + synchronous rate conversion with +/- 100ppm adjustment
  + conversion of time-stamp asynchronous to synchronous stream

SSRC synchronous rate converter, FS in/out are exchanged during STREAM_RESET
ASRC asynchronous rate converter using time-stamps (in) to synchronous FS (out) pre-LP-filtering tuned from Fout/Fin ratio + Lagrange polynomial interpolator

drift compensation managed with STREAM_SET_PARAMETER command:
TAG_CMD = 0 to stop drift compensation
TAG_CMD = 1 to continuously apply a drift compensation of +100ppm
TAG_CMD = 2 to continuously apply a drift compensation of -100ppm

The script associated to the node is used to read the in/out arcs filling state
    to tune the drift control

``` 
node arm_stream_resampler (i)

    parameters     0             ; TAG   "load all parameters"
        
        2  i8; 2 2          nb input/output arcs
        4 i16; 0 0 2 0      move arc0,chan0, to arc2,chan0
    end
end

```

----------------------------------------------------------------------------------------
# arm_stream_qos (TBD)

Operation : align the content of arc buffers with respect to an high-priority one. (HQOS)
Parameters : the list of arcs and the one taken as time-alignment reference 

```
node arm_stream_qos (i)

    parameters     0             ; TAG   "load all parameters"
TBD
    end
end
```

----------------------------------------------------------------------------------------
# arm_stream_split (TBD)

Operation : share the input arc to several output arcs, to let several nodes have
   access to the same buffer without data duplication. Consolidate the output read
   indexes before updating the input arc read index. 
   There is no priority managed between output arcs.
   The base, size and write index of the output arcs are identical to the input arc 

Parameters : the list of arcs , no memory for instances is needed

```
node 
    arm_stream_split (i)
    parameters     0             ; TAG   "load all parameters"
        
        2  i8; 2 2          nb input/output arcs
        4 i16; 0 0 2 0      move arc0,chan0, to arc2,chan0
        4 i16; 0 1 2 1      move arc0,chan1, to arc2,chan1
        4 i16; 1 0 2 2      move arc1,chan0, to arc2,chan2
        4 i16; 1 1 2 3      move arc1,chan1, to arc2,chan3
    end
end
```

----------------------------------------------------------------------------------------
# arm_stream_analysis (TBD)

 Operation : spectrum and energy analysis
 Use-case : debug with script control
            results are used with "read_parameter"

presets :
#1 : frequency estimator, counting the number of periods
 Operation : input signal frequency estimator with second-order auto-regression
 Use-case example: measure 50Hz from power lines on the three phases (a -1% deviation means "power-down").
 Parameters : none. output data estimate sampling rate is provided at reset
#2 : energy estimation and DC offset
#3 : energy analysis on 3 frequency points (Goertzel)
#4 : full spectrum 64 points in decibels

```
node arm_stream_analysis (i)

    parameters     0             ; TAG   "load all parameters"
        
    end
end
```

----------------------------------------------------------------------------------------
# sigp_stream_detector2D (TBD)

Motion and pattern detector (lines)

Operation : detection of movement(s) and computation of the movement map
Parameters : sensitivity, floor-noise smoothing factors
Metadata : decimated map of movement detection



```
node arm_stream_detector2D (i)

TBD

end
```

----------------------------------------------------------------------------------------
# arm_stream_filter2D (TBD)

Filter, rescale/zoom/extract, rotate, exposure compensation

Operation : 2D filters 
Parameters : convolutional kernels, decimation, distortion, color mapping/effect

presets:
#1 : bypass

parameter of filter : 

```
node arm_stream_filter2D   (i)

	TBD
end
```


----------------------------------------------------------------------------------------
# JPG encoder

From "bitbank"

# JPG decoder

From "EML"
