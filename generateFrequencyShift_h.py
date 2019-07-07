number = 256;
frequency_shift = 150;
samplerate = 2200;

file = fopen('freqShift%d.h' % number,'w');

file.write("#ifndef PY_FREQ_SHIFT_H");
file.write("#define PY_FREQ_SHIFT_%d_H" % number)
file.write("const double cos%d[%d] = {" % (frequency_shift,number))
for i in range(number):
	file.write(cos(2*pi*frequency_shift*i/samplerate);
	file.write(",\n")
file.write("}")
file.write("#endif");