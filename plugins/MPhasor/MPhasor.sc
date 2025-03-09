// MPhasor is an updated version of Phasor with more modern code.
MPhasor : UGen {
    *ar { arg trig = 0.0, rate = 1.0, start = 0.0, end = 1.0, resetPos = 0.0;
        ^this.multiNew('audio', trig, rate, start, end, resetPos);
    }
    *kr { arg trig = 0.0, rate = 1.0, start = 0.0, end = 1.0, resetPos = 0.0;
        ^this.multiNew('control', trig, rate, start, end, resetPos);
    }
}
