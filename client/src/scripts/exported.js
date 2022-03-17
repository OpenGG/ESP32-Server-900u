// original exploit: https://github.com/ChendoChap/pOOBs4

/* eslint-disable no-unused-vars, camelcase, new-cap, no-plusplus, no-bitwise, no-alert */
/* globals chain, int64, p, libKernelBase, OFFSET_lk_pthread_create, zLoading, zJailbreakDone */

const usbWaitTime = 5000;

const showMessage = (msg) => {
  zLoading.textContent = msg;
};

const loadPayloadData = (PLfile) =>
  new Promise((resolve, reject) => {
    const xhr = new XMLHttpRequest();
    xhr.open("GET", PLfile, true);
    xhr.overrideMimeType("text/plain; charset=x-user-defined");
    xhr.onreadystatechange = () => {
      if (xhr.readyState !== 4) {
        return;
      }

      if (xhr.status === 200) {
        resolve(xhr.response);
        return;
      }

      reject(new Error(`Failed to load ${PLfile}: ${xhr.status}`));
    };
    xhr.send();
  });

// usb functions - stooged
const disableUSB = () => {
  const xhr = new XMLHttpRequest();
  xhr.open("POST", "/admin/usb/off", true);
  xhr.send(null);
};

const enableUSB = () => {
  const xhr = new XMLHttpRequest();
  xhr.open("POST", "/admin/usb/on", true);
  xhr.send(null);
};

const injectPayload = (payloadData) => {
  // dynamic payload inject - stooged
  if (payloadData.length > 0) {
    const payload_buffer = chain.syscall(
      477,
      new int64(0x26200000, 0x9),
      0x300000,
      7,
      0x41000,
      -1,
      0
    );
    const bufLen = payloadData.length;
    const payload_loader = p.malloc32(bufLen);
    const loader_writer = payload_loader.backing;
    for (let i = 0; i < bufLen / 4; i++) {
      const hxVal = payloadData
        .slice(i * 4, 4 + i * 4)
        .split("")
        .reverse()
        .join("")
        .split("")
        .map((s) => `0000${s.charCodeAt(0).toString(16)}`.slice(-2))
        .join("");
      loader_writer[i] = parseInt(hxVal, 16);
    }
    chain.syscall(74, payload_loader, bufLen, 0x1 | 0x2 | 0x4);
    const pthread = p.malloc(0x10);
    chain.call(
      libKernelBase.add32(OFFSET_lk_pthread_create),
      pthread,
      0x0,
      payload_loader,
      payload_buffer
    );
  } else {
    alert("No Payload Data!");
  }
};

const sleep = (ms) =>
  new Promise((resolve) => {
    setTimeout(resolve, ms);
  });

const jbdone = () => {
  zJailbreakDone();
  // progress.style.display = "none";
  // mymenu.style.display = "block";
  // msgs.innerHTML = "<h1 style='font-size:25px;text-align:center;'>Menu Loaded!!!</h1>";
};

const injectBinLoader = () => {
  const payload_buffer = chain.syscall(
    477,
    0x0,
    0x300000,
    0x7,
    0x1000,
    0xffffffff,
    0
  );
  const payload_loader = p.malloc32(0x1000);
  const loader_writer = payload_loader.backing;
  loader_writer[0] = 0x56415741;
  loader_writer[1] = 0x83485541;
  loader_writer[2] = 0x894818ec;
  loader_writer[3] = 0xc748243c;
  loader_writer[4] = 0x10082444;
  loader_writer[5] = 0x483c2302;
  loader_writer[6] = 0x102444c7;
  loader_writer[7] = 0x00000000;
  loader_writer[8] = 0x000002bf;
  loader_writer[9] = 0x0001be00;
  loader_writer[10] = 0xd2310000;
  loader_writer[11] = 0x00009ce8;
  loader_writer[12] = 0xc7894100;
  loader_writer[13] = 0x8d48c789;
  loader_writer[14] = 0xba082474;
  loader_writer[15] = 0x00000010;
  loader_writer[16] = 0x000095e8;
  loader_writer[17] = 0xff894400;
  loader_writer[18] = 0x000001be;
  loader_writer[19] = 0x0095e800;
  loader_writer[20] = 0x89440000;
  loader_writer[21] = 0x31f631ff;
  loader_writer[22] = 0x0062e8d2;
  loader_writer[23] = 0x89410000;
  loader_writer[24] = 0x2c8b4cc6;
  loader_writer[25] = 0x45c64124;
  loader_writer[26] = 0x05ebc300;
  loader_writer[27] = 0x01499848;
  loader_writer[28] = 0xf78944c5;
  loader_writer[29] = 0xbaee894c;
  loader_writer[30] = 0x00001000;
  loader_writer[31] = 0x000025e8;
  loader_writer[32] = 0x7fc08500;
  loader_writer[33] = 0xff8944e7;
  loader_writer[34] = 0x000026e8;
  loader_writer[35] = 0xf7894400;
  loader_writer[36] = 0x00001ee8;
  loader_writer[37] = 0x2414ff00;
  loader_writer[38] = 0x18c48348;
  loader_writer[39] = 0x5e415d41;
  loader_writer[40] = 0x31485f41;
  loader_writer[41] = 0xc748c3c0;
  loader_writer[42] = 0x000003c0;
  loader_writer[43] = 0xca894900;
  loader_writer[44] = 0x48c3050f;
  loader_writer[45] = 0x0006c0c7;
  loader_writer[46] = 0x89490000;
  loader_writer[47] = 0xc3050fca;
  loader_writer[48] = 0x1ec0c748;
  loader_writer[49] = 0x49000000;
  loader_writer[50] = 0x050fca89;
  loader_writer[51] = 0xc0c748c3;
  loader_writer[52] = 0x00000061;
  loader_writer[53] = 0x0fca8949;
  loader_writer[54] = 0xc748c305;
  loader_writer[55] = 0x000068c0;
  loader_writer[56] = 0xca894900;
  loader_writer[57] = 0x48c3050f;
  loader_writer[58] = 0x006ac0c7;
  loader_writer[59] = 0x89490000;
  loader_writer[60] = 0xc3050fca;
  chain.syscall(74, payload_loader, 0x4000, 0x1 | 0x2 | 0x4);
  const pthread = p.malloc(0x10);
  //
  // {
  chain.fcall(window.syscalls[203], payload_buffer, 0x300000);
  chain.fcall(
    libKernelBase.add32(OFFSET_lk_pthread_create),
    pthread,
    0x0,
    payload_loader,
    payload_buffer
  );
  // }
  chain.run();
};
