[x,fs] = audioread('xbucha02.wav');  %nacteni signalu
X = fft(x);  %furierova transformace

Xshow = abs(X(1:8001));  %polovina + absolutni hodnota
plot(Xshow);  %tisknuti
xlabel('frekvence [Hz]');  %tisknuti osy x

b = [0.2324 -0.4112 0.2324];  %koeficienty b filtr
a = [1 0.2289 0.4662];  %koeficienty a filtr
xf = filter(b,a,x);  %pouziti na signal

p = roots(a);  %test na stabilnost
if (isempty(p) | abs(p) < 1) 
  disp('stabilni...')
else
  disp('NESTABILNI !!!')
end

fvtool(b,a,'polezero');  %radky pro tisknuti grafu nul a polu
[b,a] = eqtflength(b,a);
[z,p,o] = tf2zp(b,a);
text(real(z)+.1,imag(z),'Zero');
text(real(p)+.1,imag(p),'Pole');

[h,w] = freqz(b,a,256, fs);  %frekvencni charakteristika
plot (w,abs(h)); grid; xlabel('f'); ylabel('|H(f)|');

XF = fft(xf);  %fourierova transformace pro prefiltrovany signal
XFshow = abs(XF(1:8001));  %uprava pro tisk
plot(XFshow);  %tisk
xlabel('frekvence [Hz]');  %tisknuti osy x