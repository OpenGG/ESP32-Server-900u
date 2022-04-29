/* globals loadPayloadData, injectPayload, zPlStatus, zMenu, sleep, deviceSetSleep */

// eslint-disable-next-line no-unused-vars
const zMenuInit = async () => {
  const delegate = (className, fn) => (e) => {
    const { target } = e;

    if (target.classList.contains(className)) {
      fn(target);
    }
  };

  let payloadLock = false;

  const plStatusPrio = {
    className: 'pending',
    textContent: 'Please select button',
  };

  const execButton = async (btn) => {
    if (payloadLock) {
      return;
    }

    const name = btn.textContent.trim();
    //   const desc = btn.getAttribute('data-desc');
    const custom = btn.getAttribute('data-custom');
    const bin = btn.getAttribute('data-bin');
    const msg = btn.getAttribute('data-msg');

    payloadLock = true;

    zPlStatus.className = 'running';

    zPlStatus.textContent = `${name} loading... please wait`;

    try {
      if (bin) {
        const ab = await loadPayloadData(bin);
        injectPayload(ab);
      }

      if (custom) {
        // eslint-disable-next-line no-eval
        (0, eval)(`${custom}()`);
      }

      zPlStatus.className = 'done';
      zPlStatus.textContent = msg || `${name} loaded`;
    } catch (e) {
      zPlStatus.className = 'fail';
      zPlStatus.textContent = `Error running ${name}: ${e.message}`;
    }

    plStatusPrio.className = zPlStatus.className;
    plStatusPrio.textContent = zPlStatus.textContent;

    payloadLock = false;
  };

  const onClickButton = delegate('btn-payload', execButton);

  const onHoverButton = delegate('btn-payload', (btn) => {
    if (payloadLock) {
      return;
    }

    const desc = btn.getAttribute('data-desc');

    zPlStatus.className = '';
    zPlStatus.textContent = desc;
  });

  const onLeaveButton = delegate('btn-payload', () => {
    if (payloadLock) {
      return;
    }

    zPlStatus.className = plStatusPrio.className;
    zPlStatus.textContent = plStatusPrio.textContent;
  });

  const getBtn = (cls) => {
    if (!cls) {
      return null;
    }

    const btns = document.querySelectorAll('.btn-payload');

    for (let i = 0; i < btns.length; i += 1) {
      const btn = btns[i];
      if (btn.className.includes(`btn-${cls}`)) {
        return btn;
      }
    }

    return null;
  };

  const getParam = (name) => {
    const params = new URLSearchParams(window.location.hash.slice(1));

    return params.get(name);
  };

  const autoExec = async () => {
    const value = getParam('a');

    const autoBtn = value && getBtn(value);

    if (!autoBtn) {
      return;
    }

    await sleep(500);

    execButton(autoBtn);
  };

  const autoSleep = async () => {
    const value = getParam('s');

    const minutes = (value && parseInt(value, 10)) || 0;

    // >5m && < 10h
    if (minutes > 5 && minutes < 10 * 60) {
      await deviceSetSleep(minutes * 60);
    }
  };

  zMenu.addEventListener('click', onClickButton, false);
  zMenu.addEventListener('mouseover', onHoverButton, false);
  zMenu.addEventListener('mouseout', onLeaveButton, false);

  await autoExec();
  await autoSleep();
};
