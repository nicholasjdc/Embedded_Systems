
x = 0;
y = 0.1;
z = 0;
const espButton = document.querySelector('#connectESP');

espButton.addEventListener('click', async () => {
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
    console.log('VALUE:' +value);
    if (value) {
      splitValue = value.split(',')
      if(splitValue.length >=5){
        xKnobVal = parseInt(splitValue[2].split(':')[1])
        yKnobVal = parseInt(splitValue[3])
        if (!isNaN(xKnobVal)){
          xChange = 1790 - xKnobVal
          x+= xChange/100

        }
        if (!isNaN(yKnobVal)){
          yChange = 1760 - yKnobVal
          y +=yChange/100

        }
        console.log('x: ' +x)
        console.log('y: ' +y)


      }
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











var audioCtx = new AudioContext();


sampleList = ['Berck-Plage', 'Cut', 'Lady-Lazarus', 'The-Applicant', 'Tulips']
panners = new Map()
spMap = new Map()
startingPositions = [[0.1,0.1], [300,300],[-300,300],[-300,-300],[-300,300]]
const globalGain = audioCtx.createGain(); //this will control the volume of all notes
globalGain.gain.setValueAtTime(0.1, audioCtx.currentTime)
globalGain.connect(audioCtx.destination);

async function loadBuffer(bufferURL) {
  //better to have a try/catch block here, but for simplicity...
  const response = await fetch(bufferURL);
  const arrayBuffer = await response.arrayBuffer();
  const audioBuffer = await audioCtx.decodeAudioData(arrayBuffer);
  return audioBuffer;
}
window.addEventListener('keydown', keyDown, false);
  function keyDown(event) {
    const key = (event.detail || event.which).toString();
    if(key == '87'){
      y++;
    }else if(key == '83'){
      y--;
    }else if(key == '65'){
      x--;
    }else if(key == '68'){
      x++;
    }
    console.log('x: ' +x)
    console.log('y: ' +y)
}
const playButton = document.querySelector('#playButton');

playButton.addEventListener('click', async function () {
 
  const move = () => {
    const later = audioCtx.currentTime + 0.016; 
    for(const [sample, panner] of Object.entries(panners)){
      panner.positionX.value =spMap[sample][0] -x;
      panner.positionY.value  = spMap[sample][1]-y;
    }
    requestAnimationFrame(move);
  };


  for(let i=0; i<sampleList.length; i++){
    const source = audioCtx.createBufferSource();

    panners[sampleList[i]] = new PannerNode(audioCtx)
    panners[sampleList[i]].panningModel='HRTF'
    panners[sampleList[i]] .positionX.value = startingPositions[i][0]
    panners[sampleList[i]] .positionY.value = startingPositions[i][1]
    spMap[sampleList[i]] = startingPositions[i]
    samplePath = './samples/' + sampleList[i] + '.mp3'
    var audioBuffer = await loadBuffer(samplePath);
    source.connect(panners[sampleList[i]]).connect(audioCtx.destination);
    source.buffer = audioBuffer;
    source.start();

  }
  
  move();

});
