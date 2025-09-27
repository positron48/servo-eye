#ifndef UI_HTML_H
#define UI_HTML_H

#include <Arduino.h>

// Auto-generated from ui.html
// Size: 23701 bytes

const char ui_html_data[] = R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>Eye Servo Animation – ESP8266 Controller</title>
<style>
  body{margin:0; font:14px/1.4 "Roboto", system-ui, sans-serif; background:#fafafa; color:#212121;}
  .wrap{display:grid; grid-template-columns:1fr; gap:16px; padding:16px; max-width:1200px; margin:0 auto}
  @media(min-width:768px){ .wrap{grid-template-columns:1fr 1fr; gap:20px; padding:20px} }
  @media(min-width:1024px){ .wrap{gap:24px; padding:24px} }
  .card{background:#fff; border-radius:4px; box-shadow:0 2px 4px rgba(0,0,0,0.1); overflow:hidden;}
  .canvas-card{padding:0; aspect-ratio:1/1; display:flex;}
  canvas{width:100%; height:100%; display:block; cursor: grab;}
  canvas:active{cursor: grabbing;}
  .panel{padding:16px}
  @media(min-width:768px){ .panel{padding:20px} }
  h1{font-size:20px; font-weight:500; color:#212121; margin:0 0 16px 0; text-align:center;}
  .control-group{margin-bottom:16px;}
  .control-group h3{font-size:12px; font-weight:500; color:#757575; margin:0 0 8px 0; text-transform:uppercase; letter-spacing:0.5px;}
  .row{display:flex; gap:8px; flex-wrap:wrap; margin-top:8px; align-items:end;}
  .row:first-child{margin-top:0;}
  @media(max-width:767px){ .row{flex-direction:column; align-items:stretch; gap:12px} }
  label{display:flex; flex-direction:column; font-size:12px; font-weight:400; gap:4px; color:#757575; min-width:70px;}
  @media(max-width:767px){ label{min-width:auto} }
  input,select,textarea{border:1px solid #e0e0e0; border-radius:4px; padding:8px 12px; font-size:14px; transition:border-color 0.2s ease; background:#fff; font-family:inherit;}
  input:focus,select:focus,textarea:focus{outline:none; border-color:#2196f3; box-shadow:0 0 0 2px rgba(33,150,243,0.2);}
  input[type="range"]{padding:0; height:4px; background:#e0e0e0; border:none; border-radius:2px;}
  input[type="range"]::-webkit-slider-thumb{appearance:none; width:20px; height:20px; background:#2196f3; border-radius:50%; cursor:pointer; box-shadow:0 2px 4px rgba(0,0,0,0.2);}
  input[type="range"]::-moz-range-thumb{width:20px; height:20px; background:#2196f3; border-radius:50%; cursor:pointer; border:none; box-shadow:0 2px 4px rgba(0,0,0,0.2);}
  input[type="checkbox"]{width:18px; height:18px; accent-color:#2196f3;}
  .btn{padding:8px 16px; border-radius:4px; border:none; cursor:pointer; font-weight:500; font-size:14px; transition:all 0.2s ease; text-transform:uppercase; letter-spacing:0.5px;}
  .btn.primary{background:#2196f3; color:#fff; box-shadow:0 2px 4px rgba(33,150,243,0.3);}
  .btn.primary:hover{background:#1976d2; box-shadow:0 4px 8px rgba(33,150,243,0.4);}
  .btn.secondary{background:#757575; color:#fff; box-shadow:0 2px 4px rgba(117,117,117,0.3);}
  .btn.secondary:hover{background:#616161; box-shadow:0 4px 8px rgba(117,117,117,0.4);}
  .label-row{flex-direction:row; align-items:center; gap:8px; min-width:auto;}
  #csv{width:100%; min-height:180px; font-family:"Roboto Mono", ui-monospace, monospace; font-size:13px; line-height:1.4; background:#fafafa; border:1px solid #e0e0e0; border-radius:4px; padding:12px; resize:vertical;}
  #csv:focus{border-color:#2196f3; box-shadow:0 0 0 2px rgba(33,150,243,0.2); background:#fff;}
  .button-group{display:flex; gap:8px; justify-content:center; margin-top:16px; flex-wrap:wrap;}
  @media(max-width:767px){ .button-group{flex-direction:column} .btn{width:100%; text-align:center} }
  .value-display{font-weight:500; color:#2196f3; font-size:13px;}
  .status{text-align:center; margin:10px 0; padding:8px; background:#e8f5e8; border-radius:4px; color:#2e7d32;}
  .error{background:#ffebee; color:#c62828;}
  .warning{background:#fff3e0; color:#ef6c00;}
</style>
</head>
<body>
<div class="wrap">
  <div class="card canvas-card"><canvas id="cv" width="800" height="800"></canvas></div>
  <div class="card panel">
    <h1>Eye Servo Animation</h1>
    <div id="status" class="status">Connecting to ESP8266...</div>
    
    <div class="control-group">
      <h3>Position</h3>
      <div class="row">
        <label>Yaw <span id="yawLbl" class="value-display">0.0°</span><input type="range" id="yaw" min="-60" max="60" step="0.1" value="0"></label>
        <label>Pitch <span id="pitchLbl" class="value-display">0.0°</span><input type="range" id="pitch" min="-60" max="60" step="0.1" value="0"></label>
      </div>
    </div>

    <div class="control-group">
      <h3>Limits</h3>
      <div class="row">
        <label>Min Yaw <input id="minYaw" type="number" value="-60"></label>
        <label>Max Yaw <input id="maxYaw" type="number" value="60"></label>
        <label>Min Pitch <input id="minPitch" type="number" value="-60"></label>
        <label>Max Pitch <input id="maxPitch" type="number" value="60"></label>
      </div>
    </div>

    <div class="control-group">
      <h3>Animation</h3>
      <div class="row">
        <label>Preset <select id="preset">
          <option>Center Sweep</option>
          <option>Figure Eight</option>
          <option>Blink Look</option>
          <option>Circle</option>
        </select></label>
        <label>Speed <span id="speedLbl" class="value-display">1.0</span><input id="speed" type="range" min="0.1" max="3" step="0.1" value="1"></label>
        <label class="label-row"><input type="checkbox" id="loop" checked> Loop</label>
      </div>
    </div>

    <div class="control-group">
      <h3>Data</h3>
      <textarea id="csv" rows="8" placeholder="yaw,pitch,duration"></textarea>
    </div>

    <div class="button-group">
      <button class="btn primary" id="run">Run</button>
      <button class="btn secondary" id="stop">Stop</button>
      <button class="btn secondary" id="crazy">Chaos</button>
    </div>
  </div>
  
  <div class="card panel">
    <h1>System Information</h1>
    <div id="otaStatus" class="status">Loading system info...</div>
    
    <div class="control-group">
      <h3>Device Info</h3>
      <div class="row">
        <label>Hostname <span id="hostname" class="value-display">-</span></label>
        <label>IP Address <span id="ipAddress" class="value-display">-</span></label>
        <label>MAC Address <span id="macAddress" class="value-display">-</span></label>
        <label>Version <span id="version" class="value-display">-</span></label>
      </div>
    </div>

    <div class="control-group">
      <h3>System Status</h3>
      <div class="row">
        <label>Uptime <span id="uptime" class="value-display">-</span></label>
        <label>Free Heap <span id="freeHeap" class="value-display">-</span></label>
        <label>Flash Size <span id="flashSize" class="value-display">-</span></label>
        <label>Flash Used <span id="flashUsed" class="value-display">-</span></label>
      </div>
    </div>

    <div class="control-group">
      <h3>OTA Update</h3>
      <div class="row">
        <label>OTA Hostname <span class="value-display">eye.local</span></label>
        <label>OTA Port <span class="value-display">8266</span></label>
        <label>OTA Password <span class="value-display">12345678</span></label>
      </div>
      <div style="margin-top: 16px; padding: 12px; background: #f5f5f5; border-radius: 4px; font-size: 13px; line-height: 1.4;">
        <strong>Instructions for OTA Update:</strong><br>
        1. Connect to WiFi network "EyeController" (password: 12345678)<br>
        2. Use PlatformIO: <code>pio run -e d1_mini_ota -t upload</code><br>
        3. Or Arduino IDE: select "eye.local at 192.168.4.1"<br>
        4. Use password "12345678" for OTA authentication
      </div>
    </div>

    <div class="button-group">
      <button class="btn secondary" id="refreshInfo">Refresh Info</button>
    </div>
  </div>
</div>
<script>
(function(){
  const cv=document.getElementById('cv');
  const ctx=cv.getContext('2d');
  const deg2rad=d=>d*Math.PI/180; const clamp=(v,min,max)=>Math.min(max,Math.max(min,v));

  const yawEl=document.getElementById('yaw');
  const pitchEl=document.getElementById('pitch');
  const yawLbl=document.getElementById('yawLbl');
  const pitchLbl=document.getElementById('pitchLbl');
  const minYawEl=document.getElementById('minYaw');
  const maxYawEl=document.getElementById('maxYaw');
  const minPitchEl=document.getElementById('minPitch');
  const maxPitchEl=document.getElementById('maxPitch');
  const speedEl=document.getElementById('speed');
  const speedLbl=document.getElementById('speedLbl');
  const csvEl=document.getElementById('csv');
  const runBtn=document.getElementById('run');
  const stopBtn=document.getElementById('stop');
  const crazyBtn=document.getElementById('crazy');
  const presetEl=document.getElementById('preset');
  const statusEl=document.getElementById('status');
  
  // OTA elements
  const otaStatusEl=document.getElementById('otaStatus');
  const hostnameEl=document.getElementById('hostname');
  const ipAddressEl=document.getElementById('ipAddress');
  const macAddressEl=document.getElementById('macAddress');
  const versionEl=document.getElementById('version');
  const uptimeEl=document.getElementById('uptime');
  const freeHeapEl=document.getElementById('freeHeap');
  const flashSizeEl=document.getElementById('flashSize');
  const flashUsedEl=document.getElementById('flashUsed');
  const refreshInfoBtn=document.getElementById('refreshInfo');

  const PRESETS={
    'Center Sweep':`-60,0,600\n-30,0,600\n0,0,600\n30,0,600\n60,0,600\n0,0,600`,
    'Figure Eight':`-40,20,500\n0,0,500\n40,-20,500\n0,0,500\n-40,-20,500\n0,0,500\n40,20,500\n0,0,500`,
    'Blink Look':`0,15,300\n0,-15,300\n0,0,300\n30,0,500\n-30,0,500\n0,0,500`,
    'Circle':`30,0,200\n28,11,200\n21,21,200\n11,28,200\n0,30,200\n-11,28,200\n-21,21,200\n-28,11,200\n-30,0,200\n-28,-11,200\n-21,-21,200\n-11,-28,200\n0,-30,200\n11,-28,200\n21,-21,200\n28,-11,200`
  };

  const state={yaw:0,pitch:0,minYaw:-60,maxYaw:60,minPitch:-60,maxPitch:60,speed:1,loop:true,R:300,anim:null,irisAngle:0.42,pupilAngle:0.14};

  // Throttling for position updates (200ms)
  let lastPositionUpdate = 0;
  let pendingPosition = null;
  const POSITION_THROTTLE_MS = 200;

  // API functions
  async function sendPosition(yaw, pitch) {
    const now = Date.now();
    pendingPosition = {yaw, pitch};
    
    if (now - lastPositionUpdate < POSITION_THROTTLE_MS) {
      return; // Skip this update, will be sent by timeout
    }
    
    await sendPositionImmediate(yaw, pitch);
  }

  async function sendPositionImmediate(yaw, pitch) {
    try {
      const response = await fetch('/api/position', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: `yaw=${yaw}&pitch=${pitch}`
      });
      if (!response.ok) throw new Error('Network error');
      updateStatus('Connected to ESP8266', 'status');
      lastPositionUpdate = Date.now();
      
      // Check if there's a pending position to send
      if (pendingPosition && (pendingPosition.yaw !== yaw || pendingPosition.pitch !== pitch)) {
        setTimeout(() => {
          if (pendingPosition) {
            sendPositionImmediate(pendingPosition.yaw, pendingPosition.pitch);
            pendingPosition = null;
          }
        }, POSITION_THROTTLE_MS);
      } else {
        pendingPosition = null;
      }
    } catch (error) {
      updateStatus('Connection error: ' + error.message, 'error');
    }
  }

  async function startAnimation() {
    try {
      const response = await fetch('/api/animation', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: `action=start&data=${encodeURIComponent(csvEl.value)}`
      });
      if (!response.ok) throw new Error('Network error');
      updateStatus('Animation started', 'status');
    } catch (error) {
      updateStatus('Animation error: ' + error.message, 'error');
    }
  }

  async function stopAnimation() {
    try {
      const response = await fetch('/api/animation', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: 'action=stop'
      });
      if (!response.ok) throw new Error('Network error');
      updateStatus('Animation stopped', 'status');
    } catch (error) {
      updateStatus('Stop error: ' + error.message, 'error');
    }
  }

  async function saveSettings() {
    try {
      const response = await fetch('/api/settings', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: `preset=${encodeURIComponent(presetEl.value)}&speed=${speedEl.value}&loop=${document.getElementById('loop').checked}`
      });
      if (!response.ok) throw new Error('Network error');
      updateStatus('Settings saved', 'status');
    } catch (error) {
      updateStatus('Save error: ' + error.message, 'error');
    }
  }

  async function loadSettings() {
    try {
      const response = await fetch('/api/settings');
      if (!response.ok) throw new Error('Network error');
      const data = await response.json();
      
      presetEl.value = data.preset || 'Center Sweep';
      speedEl.value = data.speed || 1;
      document.getElementById('loop').checked = data.loop !== false;
      
      csvEl.value = PRESETS[presetEl.value] || '';
      updateSpeedLabel();
      updateStatus('Settings loaded', 'status');
    } catch (error) {
      updateStatus('Load error: ' + error.message, 'error');
    }
  }

  async function loadPosition() {
    try {
      const response = await fetch('/api/position');
      if (!response.ok) throw new Error('Network error');
      const data = await response.json();
      
      state.yaw = data.yaw || 0;
      state.pitch = data.pitch || 0;
      yawEl.value = state.yaw;
      pitchEl.value = state.pitch;
      updateLabels();
      updateStatus('Position loaded', 'status');
    } catch (error) {
      updateStatus('Position load error: ' + error.message, 'error');
    }
  }

  async function loadOTAInfo() {
    try {
      const response = await fetch('/api/ota/status');
      if (!response.ok) throw new Error('Network error');
      const data = await response.json();
      
      hostnameEl.textContent = data.hostname || '-';
      ipAddressEl.textContent = data.ip || '-';
      macAddressEl.textContent = data.mac || '-';
      versionEl.textContent = data.version || '-';
      uptimeEl.textContent = formatUptime(data.uptime || 0);
      freeHeapEl.textContent = formatBytes(data.freeHeap || 0);
      flashSizeEl.textContent = formatBytes(data.flashSize || 0);
      flashUsedEl.textContent = formatBytes(data.flashUsed || 0);
      
      updateOTAStatus('System info loaded', 'status');
    } catch (error) {
      updateOTAStatus('System info error: ' + error.message, 'error');
    }
  }

  function formatUptime(seconds) {
    const hours = Math.floor(seconds / 3600);
    const minutes = Math.floor((seconds % 3600) / 60);
    const secs = seconds % 60;
    return `${hours}h ${minutes}m ${secs}s`;
  }

  function formatBytes(bytes) {
    if (bytes === 0) return '0 B';
    const k = 1024;
    const sizes = ['B', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(1)) + ' ' + sizes[i];
  }

  function updateOTAStatus(message, type = 'status') {
    otaStatusEl.textContent = message;
    otaStatusEl.className = `status ${type}`;
    if (type === 'error' || type === 'warning') {
      setTimeout(() => {
        otaStatusEl.textContent = 'System info loaded';
        otaStatusEl.className = 'status';
      }, 3000);
    }
  }

  function updateStatus(message, type = 'status') {
    statusEl.textContent = message;
    statusEl.className = `status ${type}`;
    if (type === 'error' || type === 'warning') {
      setTimeout(() => {
        statusEl.textContent = 'Connected to ESP8266';
        statusEl.className = 'status';
      }, 3000);
    }
  }

  function updateSpeedLabel() {
    speedLbl.textContent = speedEl.value;
  }

  function updateLabels() {
    yawLbl.textContent = state.yaw.toFixed(1) + "°";
    pitchLbl.textContent = state.pitch.toFixed(1) + "°";
  }

  // Initialize
  csvEl.value=PRESETS['Center Sweep'];
  presetEl.onchange=()=>{
    csvEl.value=PRESETS[presetEl.value]||'';
    saveSettings();
  };

  function sync(){
    state.minYaw=+minYawEl.value; state.maxYaw=+maxYawEl.value;
    state.minPitch=+minPitchEl.value; state.maxPitch=+maxPitchEl.value;
    state.yaw=clamp(+yawEl.value,state.minYaw,state.maxYaw);
    state.pitch=clamp(+pitchEl.value,state.minPitch,state.maxPitch);
    updateLabels();
    state.speed=+speedEl.value; updateSpeedLabel();
    state.loop=document.getElementById('loop').checked;
    
    // Send position to ESP8266
    sendPosition(state.yaw, state.pitch);
  }

  ;['input','change'].forEach(ev=>{
    [yawEl,pitchEl,minYawEl,maxYawEl,minPitchEl,maxPitchEl,speedEl,document.getElementById('loop')].forEach(el=>el.addEventListener(ev,sync));
  });

  // Animation functions
  function parseCSV(t){return t.split(/\n/).map(l=>l.trim()).filter(Boolean).map(l=>{let[a,b,c]=l.split(/[,;\s]+/);return{yaw:+a,pitch:+b,dur:+c}})}

  function startAnim(){
    const q=parseCSV(csvEl.value).map(s=>({yaw:clamp(s.yaw,state.minYaw,state.maxYaw),pitch:clamp(s.pitch,state.minPitch,state.maxPitch),dur:s.dur}));
    if(!q.length)return; 
    state.anim={queue:q,i:0,t0:performance.now(),running:true}; 
    startAnimation();
    requestAnimationFrame(stepAnim);
  }

  function stepAnim(now){
    const st=state.anim;if(!st||!st.running)return;
    const cur=st.queue[st.i];
    const prev=st.queue[(st.i-1+st.queue.length)%st.queue.length]||cur;
    const t=(now-st.t0)*state.speed;
    const prog=Math.min(1,t/cur.dur);
    state.yaw=prev.yaw+(cur.yaw-prev.yaw)*prog;
    state.pitch=prev.pitch+(cur.pitch-prev.pitch)*prog;
    yawEl.value=state.yaw.toFixed(1); pitchEl.value=state.pitch.toFixed(1);
    updateLabels();
    draw();
    if(prog>=1){st.i++;st.t0=now;if(st.i>=st.queue.length){if(state.loop)st.i=0;else{st.running=false;stopAnimation();return;}}}
    requestAnimationFrame(stepAnim);
  }  

  function stopAnim(){
    if(state.anim)state.anim.running=false;
    stopAnimation();
  }

  function crazyAnim(){
    state.anim={running:true};
    let phase='jump',t0=performance.now();let sx=state.yaw,sy=state.pitch;let tx=randomYaw(),ty=randomPitch();let holdFor=200;
    function randomYaw(){return clamp((Math.random()*(state.maxYaw-state.minYaw)+state.minYaw),state.minYaw,state.maxYaw);} 
    function randomPitch(){return clamp((Math.random()*(state.maxPitch-state.minPitch)+state.minPitch),state.minPitch,state.maxPitch);} 
    function tick(now){
      if(!state.anim.running)return;
      const dt=(now-t0)*state.speed;
      if(phase==='jump'){
        const prog=Math.min(1,dt/120);
        state.yaw=sx+(tx-sx)*prog;
        state.pitch=sy+(ty-sy)*prog;
        yawEl.value=state.yaw.toFixed(1); pitchEl.value=state.pitch.toFixed(1);
        updateLabels();
        draw();
        sendPosition(state.yaw, state.pitch);
        if(prog>=1){phase='hold';t0=now;holdFor=200+Math.random()*600;}
      }else{
        draw();
        if(dt>=holdFor){phase='jump';t0=now;sx=state.yaw;sy=state.pitch;tx=randomYaw();ty=randomPitch();}
      }
      requestAnimationFrame(tick);
    }
    requestAnimationFrame(tick);
  } 

  runBtn.onclick=startAnim; stopBtn.onclick=stopAnim; crazyBtn.onclick=crazyAnim;
  refreshInfoBtn.onclick=loadOTAInfo;

  // Pointer drag to control gaze
  let isDrag=false, activeId=null;
  function setGazeFromPointer(e){
    const rect=cv.getBoundingClientRect();
    const sx=cv.width/rect.width, sy=cv.height/rect.height;
    const px=(e.clientX-rect.left)*sx;
    const py=(e.clientY-rect.top)*sy;
    const cx=cv.width/2, cy=cv.height/2, R=state.R;
    const dx=px-cx, dy=py-cy;
    const r2=dx*dx+dy*dy;
    if(!isDrag){ if(r2>R*R) return; }
    const k = R*Math.cos(state.irisAngle);
    let gx0 = dx / k;
    let gy0 = -dy / k;
    gx0 = clamp(gx0, -1, 1);
    gy0 = clamp(gy0, -1, 1);
    let pitchDeg = -Math.asin(gy0)*180/Math.PI;
    pitchDeg = clamp(Math.round(pitchDeg*10)/10, state.minPitch, state.maxPitch);
    const cp = Math.cos(deg2rad(pitchDeg));
    let yawDeg;
    if(cp < 1e-6){
      yawDeg = state.yaw;
    }else{
      let sinYaw = gx0 / cp;
      sinYaw = clamp(sinYaw, -1, 1);
      yawDeg = Math.asin(sinYaw)*180/Math.PI;
    }
    yawDeg = clamp(Math.round(yawDeg*10)/10, state.minYaw, state.maxYaw);
    state.yaw = yawDeg;
    state.pitch = pitchDeg;
    yawEl.value=state.yaw.toFixed(1); pitchEl.value=state.pitch.toFixed(1);
    updateLabels();
    draw();
    sendPosition(state.yaw, state.pitch);
  }

  cv.addEventListener('pointerdown', (e)=>{
    stopAnim();
    isDrag=true; activeId=e.pointerId;
    if(cv.setPointerCapture) cv.setPointerCapture(e.pointerId);
    cv.style.cursor='grabbing';
    setGazeFromPointer(e);
  });
  cv.addEventListener('pointermove', (e)=>{ if(!isDrag || e.pointerId!==activeId) return; setGazeFromPointer(e); });
  function endDrag(e){
    if(!isDrag || (e && e.pointerId!==activeId)) return;
    isDrag=false; activeId=null;
    if(cv.releasePointerCapture && e) cv.releasePointerCapture(e.pointerId);
    cv.style.cursor='grab';
  }
  cv.addEventListener('pointerup', endDrag);
  cv.addEventListener('pointercancel', endDrag);
  cv.addEventListener('pointerleave', endDrag);

  function draw(){
    const w=cv.width,h=cv.height; ctx.clearRect(0,0,w,h);
    const cx=w/2, cy=h/2, R=state.R;
    const TAU=Math.PI*2;
    const vec3=(x=0,y=0,z=0)=>({x,y,z});
    const add=(a,b)=>vec3(a.x+b.x,a.y+b.y,a.z+b.z);
    const mul=(a,s)=>vec3(a.x*s,a.y*s,a.z*s);
    const norm=v=>{const L=Math.hypot(v.x,v.y,v.z)||1; return mul(v,1/L)};
    const cross=(a,b)=>vec3(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);
    function gazeVec(pitchDeg,yawDeg){
      const p=deg2rad(pitchDeg), y=deg2rad(yawDeg);
      const cp=Math.cos(p), sp=Math.sin(p); const cy=Math.cos(y), sy=Math.sin(y);
      return norm(vec3(sy*cp, -sp, cy*cp));
    }
    function sphericalDisk2D(g, th, steps, R){
      const up = Math.abs(g.z) < 0.99 ? vec3(0,0,1) : vec3(0,1,0);
      const a = norm(cross(up, g));
      const b = norm(cross(g, a));
      const pts=[];
      for(let i=0;i<steps;i++){
        const t = i/steps*TAU;
        const dir = norm(add(mul(g, Math.cos(th)), add(mul(a, Math.sin(th)*Math.cos(t)), mul(b, Math.sin(th)*Math.sin(t)))));
        const p = mul(dir, R);
        pts.push([p.x, p.y]);
      }
      return pts;
    }
    function drawDisk(g, ang, fillStyle){
      const pts = sphericalDisk2D(g, ang, 128, R);
      for(let i=0;i<pts.length;i++){ pts[i][0]+=cx; pts[i][1]=cy-pts[i][1]; }
      ctx.beginPath(); ctx.moveTo(pts[0][0], pts[0][1]);
      for(let i=1;i<pts.length;i++) ctx.lineTo(pts[i][0], pts[i][1]);
      ctx.closePath(); ctx.fillStyle = fillStyle; ctx.fill();
    }
    ctx.fillStyle='#ffffff';
    ctx.beginPath(); ctx.arc(cx,cy,R,0,Math.PI*2); ctx.fill();
    ctx.strokeStyle='#cfd4dc'; ctx.lineWidth=Math.max(1,R*0.015);
    ctx.beginPath(); ctx.arc(cx,cy,R-ctx.lineWidth*0.5,0,Math.PI*2); ctx.stroke();
    const g = gazeVec(state.pitch, state.yaw);
    drawDisk(g, state.irisAngle, '#27699e');
    drawDisk(g, state.pupilAngle, '#000');
    ctx.fillStyle='#111'; ctx.font=Math.round(R*0.09)+"px system-ui, sans-serif";
    ctx.textAlign='center'; ctx.fillText(`yaw ${state.yaw.toFixed(1)}°, pitch ${state.pitch.toFixed(1)}°`, cx, cy+R*1.25);
  }
  
  function loop(){draw();requestAnimationFrame(loop);}
  
  // Initialize
  sync();
  loadSettings();
  loadPosition();
  loadOTAInfo();
  loop();
})();
</script>
</body>
</html>
)rawliteral";

const size_t ui_html_size = 23701;

#endif // UI_HTML_H
