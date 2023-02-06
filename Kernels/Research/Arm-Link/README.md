# CMSIS-Stream

-	CMSIS-Stream is a scheduler of DSP/ML software components using an interface standard. CMSIS-Stream will be open-source in Apache license, and portable to Cortex-M, Cortex-R, Cortex-A and Laptop computers.
	Example of scheduled software components : image and voice codec, data conditioning, motion classifiers, data mixers, etc ...
-	**From the developer point of view**, it creates opaque memory interfaces to the input/output streams, and arranges data are exchanged in the desired formats of each component. CMSIS-Stream manages the memory mapping with speed constraints provided by the developer at instance creation. This lets software run with maximum performance in rising situations of memory bounded problems. CMSIS-Stream will accept code in binary format and software protection with keys. In future releases CMSIS-Stream will provide memory protection between software components. 
-	**From the system integrator view**, it eases the tuning and the replacement of one component by another one, and is made to ease processing split with multiprocessors. The stream is described with a graph (a text file) which can be designed with a graphical tool. The development of DSP/ML processing will be possible without need to write code, and allow graph adaptation and tuning without recompilation.


