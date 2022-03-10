import Queue from "./Queue";

const getMessage = (e) => {
    if (typeof e === 'string') {
        return e;
    }

    return e.message || e.reason || `${e.error}` || `${e}`;
}

const loadJS = (url) => new Promise((resolve, reject) => {
    const script = document.createElement('script')
    script.src = url
    script.async = false
    script.onload = () => {
        resolve()
    }
    script.onerror = (e) => {
        reject(new Error(`Failed to load ${url}: ${getMessage(e)}`))
    }
    document.body.appendChild(script)
});

const loadCSS = (url) => new Promise((resolve, reject) => {
    const link = document.createElement('link')
    link.href = url
    link.rel = 'stylesheet'
    link.onload = () => {
        resolve()
    }
    link.onerror = (e) => {
        reject(new Error(`Failed to load ${url}: ${getMessage(e)}`))
    }
    document.body.appendChild(link)
});


export const loadScript = (url) => {
    const isJS = url.endsWith('.js')

    if (isJS) {
        return loadJS(url);
    }

    const isCSS = url.endsWith('.css');

    if (isCSS) {
        return loadCSS(url);
    }

    throw new Error('Unknown file type');
}

// limited to concurrency of 2, prevent esp32 crashing
const loadScripts = (urls) => {
    const q = Queue(loadScript, 3);
    const promises = urls.map((url) => {
        return q.push(url);
    })
    return Promise.all(promises);
}

export default loadScripts