/* globals loadPayloadData, injectPayload, zPlStatus, zMenu */

// eslint-disable-next-line no-unused-vars
const zMenuInit = () => {
  const zProxy = (className, fn) => (e) => {
    const { target } = e;

    if (target.classList.contains(className)) {
      fn(target);
    }
  };

  let zPayloadLock = false;

  const zPlStatusPrio = {
    className: "pending",
    textContent: "Please select button",
  };

  const zOnClickButton = zProxy("btn-payload", async (btn) => {
    const name = btn.textContent.trim();
    //   const desc = btn.getAttribute('data-desc');
    const custom = btn.getAttribute("data-custom");
    const bin = btn.getAttribute("data-bin");
    const msg = btn.getAttribute("data-msg");

    zPayloadLock = true;

    zPlStatus.className = "running";

    zPlStatus.textContent = `${name} loading... please wait`;

    try {
      if (bin) {
        const ab = await loadPayloadData(bin);
        injectPayload(ab);
      }

      if (custom) {
        window[custom]();
      }

      zPlStatus.className = "done";
      zPlStatus.textContent = msg || `${name} loaded`;
    } catch (e) {
      zPlStatus.className = "fail";
      zPlStatus.textContent = `Error running ${name}: ${e.message}`;
    }

    zPlStatusPrio.className = zPlStatus.className;
    zPlStatusPrio.textContent = zPlStatus.textContent;

    zPayloadLock = false;
  });

  const zOnHoverButton = zProxy("btn-payload", (btn) => {
    if (zPayloadLock) {
      return;
    }

    const desc = btn.getAttribute("data-desc");

    zPlStatus.className = "";
    zPlStatus.textContent = desc;
  });

  const zOnLeaveButton = zProxy("btn-payload", () => {
    if (zPayloadLock) {
      return;
    }

    zPlStatus.className = zPlStatusPrio.className;
    zPlStatus.textContent = zPlStatusPrio.textContent;
  });

  zMenu.addEventListener("click", zOnClickButton, false);
  zMenu.addEventListener("mouseover", zOnHoverButton, false);
  zMenu.addEventListener("mouseout", zOnLeaveButton, false);
};
