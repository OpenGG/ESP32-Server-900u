export const ready = (update) => new Promise((resolve, reject) => {
    const {
        applicationCache
    } = window

    if (!applicationCache) {
        reject(new Error("ApplicationCache not supported"));
        return;
    }

    let to = setTimeout(resolve, 1000);

    const onUpdate = (e) => {
        clearTimeout(to);

        update(e.loaded / e.total);
    };

    const onStart = () => {
        clearTimeout(to);
    };

    const onFinish = () => {
        resolve();
    };

    applicationCache.addEventListener('downloading', onStart, false);
    applicationCache.addEventListener('progress', onUpdate, false);
    applicationCache.addEventListener('cached', onFinish, false);
});

