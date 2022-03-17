/* globals zJbStatus, zLoading, zApp, zMenuInit, poc */
let zStartTime = 0;
let zJailbreak = false;

// eslint-disable-next-line no-unused-vars
const zJailbreakDone = () => {
  if (zJailbreak) {
    return;
  }

  zJailbreak = true;
  const now = Date.now();
  const dur = ((now - zStartTime) / 1000).toFixed(1);

  zJbStatus.classList.remove("running");
  zJbStatus.classList.add("done");
  zJbStatus.textContent = `success in ${dur}s.`;

  zLoading.classList.add("hide");
  zApp.classList.add("show");

  zMenuInit();
};

const zMain = () => {
  zStartTime = Date.now();

  poc();
};

window.addEventListener(
  "load",
  () => {
    setTimeout(zMain, 1500);
  },
  false
);
