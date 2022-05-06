/*
  ==============================================================================

    ToDo.cpp Created: 17 May 2021 6:43:34pm Author:  Brian Arne

  ==============================================================================
*/

/*
 // IDEAS //
 - maybe make the color change as the tempo increases
 
 - Set the moving component class to move faster depending on the tempo
 
  - 150 pixels wide
 
  - On tempo update, need to update the PongComponent with the new move value
 

 // EXAMPLE CALCULATIONS //
 Tempo is clicks per 60 seconds.
    Divide tempo by 60 to figure out the time interval between clicks
    
 Interval * 1000 is how many miliseconds we have to traverse 150 pixels
 
 We have 100 frames per second.
     * 1 frames per ~ 10milisecond.
 
 (pixelTime){} miliInterval / 150 pixels is the number of miliseconds per pixel we need to move
 
 for every 10 ms of pixelTime, we need to move the rectangle 1 pixel
    - keep a cumulative tab so we can double move if accumulate more
 
 
 WORK ON:
    - At fast speeds we basically only get 1 update
        * Moving in chunks
        * calculation might be wrong
    - Something causing the line to not move at all at lower speeds
    - Maybe something off in the calculations
    - Running calculations while the Metronome is off, causeing getX() to be way to high
 
New Funcs:
  milliseconds to how many samples 
  44,100 samples per second
  44,100 / 1000 = samples per millisecond
  44.1 samples per milliscond ( How do we handle rounding? )
  *MainComponent already has samplesPerClick
    ** This should pass to SamplePlayback
 


New Classes:
public:
  SamplePlayback(AudioBuffer<float> sampleBuffer, int samplesPerClick)

  void processBuffer(AudioBuffer<float>* buffer);
  updateTempo(int samplesPerClikc);

private:
  void updateSamplePointer(int readSamples);



 
 */
