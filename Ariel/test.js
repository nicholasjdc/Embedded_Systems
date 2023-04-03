//when the user clicks anywhere on the page
async function playSound(soundID){ // x,y, z, sample, rest
    if(activeSounds[soundID]){
        let soundData = activeSounds[soundID]
        let activePanner = soundData['panner']
        activePanner.positionX.value = parseFloat(soundData['x']) - walkerPosition['x']
        activePanner.positionY.value = parseFloat(soundData['y']) - walkerPosition['y']
        activePanner.positionZ.value = parseFloat(soundData['z']) - walkerPosition['z']

        const source = audioCtx.createBufferSource();
        source.buffer = soundData['audioBuffer']
        source.connect(activePanner)
        source.start(audioCtx.currentTime);
        let sleepValue = soundData['sleep']
        setTimeout(function(){playSound(soundID)}, sleepValue*1000)
    }

    //Recursive function, called every rest
    
    
}


function initAudio(){
    audioCtx = new AudioContext()
}
async function loadBuffer(bufferURL) {
    try{
      const response = await fetch(bufferURL);
      const arrayBuffer = await response.arrayBuffer();
      const audioBuffer = await audioCtx.decodeAudioData(arrayBuffer);
      return audioBuffer;
    }catch{
      console.log("Error in loading Buffer")
  
      return null;
    }
}
async function loadSample(bufferURL){
    sampleRoute = 'samples/' + bufferURL + ".wav"
    sample = await loadBuffer(sampleRoute)
    return sample
}
document.addEventListener('click', async () => {
    // Prompt user to select any serial port.
    var port = await navigator.serial.requestPort();
    // be sure to set the baudRate to match the ESP32 code
    await port.open({ baudRate: 115200 });
  
    let decoder = new TextDecoderStream();
    inputDone = port.readable.pipeTo(decoder.writable);
    inputStream = decoder.readable;
  
    reader = inputStream.getReader();
    readLoop();
  
  });
  
  var inputButton = false;
  
  async function readLoop() {
    while (true) {
      const { value, done } = await reader.read();
      if (done) {
        // Allow the serial port to be closed later.
        console.log("closing connection")
        reader.releaseLock();
        break;
      }
      console.log(value);
      if (value) {
        parsedVal = parseInt(value);
        if (!isNaN(parsedVal) && parsedVal == 1) {
          inputButton = true;
        }
        else {
          inputButton = false;
        }
  
      }
    }
  };
  
  
