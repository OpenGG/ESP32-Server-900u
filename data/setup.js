let Z_WAIT_FOR_USB = 10 * 1000;
let Z_WAIT_FOR_USB_EJECT = 5 * 1000;
let Z_WAIT_FOR_NEXT_BIN = 1000;
let Z_WAIT_FOR_FAIL = 10 * 1000;

let Z_BINS = 'goldhen_2.1.2_900.bin';

const zTexts = {
    payloadWarm: '【整体进度  10%】加载完毕, 缓存预热中, 请等待...',
    exploitWait: '【整体进度  20%】缓存预热完毕, 内核破解中, 请等待...',
    usbWait: '【整体进度  50%】内核破解完毕，正在模拟 U 盘, 请等待...',
    usbDoneWait: '【整体进度  60%】正在模拟拔出 U 盘, 请等待...',
    usbDone: '【整体进度  70%】U 盘移除成功, 请等待...',
    payloadInject: '【整体进度  80%】正在注入代码, 请等待...',
    success: '【整体进度 100%】破解完成, 请关闭浏览器',
    error: '出错了，请重启后重试',
};


const zUtils = {
    wait(timeout) {
        return new Promise((resolve) => {
            setTimeout(resolve, timeout);
        });
    },
    ajax(method, url, responseType) {
        return new Promise((resolve, reject) => {
            const xhr = new XMLHttpRequest();
            xhr.responseType = responseType || 'text';
            xhr.open(method || 'GET', url, true);
            xhr.send();
            xhr.onreadystatechange = () => {
                if (xhr.readyState !== 4) {
                    return;
                }
                if (xhr.status !== 200) {
                    reject(new Error(`加载失败: ${url}`));
                    return;
                }
                resolve(xhr.response);
            };
        });
    },
    get(url, responseType) {
        return zUtils.ajax('GET', url, responseType);
    },
    post(url, responseType) {
        return zUtils.ajax('POST', url, responseType);
    },
    getArrayBuffer(url) {
        return zUtils.get(url, 'arraybuffer');
    },
    enableUSB() {
        return zUtils.post('/admin/usb/on');
    },
    disableUSB() {
        return zUtils.post('/admin/usb/off');
    },
    error(text) {
        zLog.error(text);
    },
    log(text) {
        zLog.log(text);
    },
};


const zExploit = {
    executePayload(PLS) {
        const payloadBuffer = chain.syscall(477, 0, 0x300000, 7, 0x1002, -1, 0);
        const pl = p.array_from_address(payloadBuffer, PLS.byteLength * 4);
        const buf = new Uint32Array(PLS);
        pl.set(buf, 0);
        const pthread = p.malloc(0x10);
        chain.call(
            libKernelBase.add32(OFFSET_lk_pthread_create),
            pthread,
            0x0,
            payloadBuffer,
            0
        );
    },
    async injectPayload(url) {
        const ab = await zUtils.getArrayBuffer(url);

        zExploit.executePayload(ab);
    },
};

const zMsgTexts = {
    insertUSB: 'Insert USB Drive',
    success: 'Jailbreak Done!',
    fail: 'Failed to trigger the exploit',
};

const zMsgHandles = {
    async [zMsgTexts.insertUSB]() {
        zUtils.log(zTexts.usbWait);

        await zUtils.enableUSB();
        await zUtils.wait(Z_WAIT_FOR_USB);
    },
    async [zMsgTexts.success]() {
        zUtils.log(zTexts.usbDoneWait);

        try {
            await zUtils.disableUSB();
        } catch (e) {
            // ignore
        }

        await zUtils.wait(Z_WAIT_FOR_USB_EJECT);

        zUtils.log(zTexts.usbDone);
    },
    async [zMsgTexts.fail](rawMessage) {
        zUtils.error(zTexts.error);
        zUtils.error(rawMessage);
        await zUtils.wait(Z_WAIT_FOR_FAIL);
    }
};

const zMsg = (msg) => {
    const m = `${msg}`;

    const handle = Object.keys(zMsgHandles)
        .find(k => m.startsWith(k));

    if (handle) {
        return zMsgHandles[handle](m);
    }

    throw new Error(msg);
};

const zSetup = async () => {
    try {
        const bins = Z_BINS.split(',')
            .map(a => a.trim())
            .filter(a => a);
        const binLen = bins.length;

        zUtils.log(zTexts.payloadWarm);

        for (let i = 0; i < binLen; i += 1) {
            const url = bins[i];

            await zUtils.getArrayBuffer(url);
        }

        zUtils.log(zTexts.exploitWait);

        // wait for log render
        await zUtils.wait(100);

        await poc();

        zUtils.log(`${zTexts.payloadInject} [0/${binLen}]`);

        for (let i = 0; i < binLen; i += 1) {
            const url = bins[i];

            await zExploit.injectPayload(url);

            await zUtils.wait(Z_WAIT_FOR_NEXT_BIN);

            zUtils.log(`${zTexts.payloadInject} [${i + 1}/${binLen}]`);
        }

        zUtils.log(zTexts.success);

    } catch (e) {
        zUtils.error(zTexts.error);
        zUtils.error(e.message);
    }
};
