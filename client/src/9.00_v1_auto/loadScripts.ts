const loadJS = (url: string) => new Promise<void>((resolve, reject) => {
    const script = document.createElement('script')
    script.src = url
    script.async = false
    script.onload = () => {
        resolve()
    }
    script.onerror = (e: Event | string) => {
        const msg = typeof e === 'string' ? e : 'load failed'

        reject(new Error(`Failed to load ${url}: ${msg}`))
    }
    document.body.appendChild(script)
});

const loadCSS = (url: string) => new Promise<void>((resolve, reject) => {
    const link = document.createElement('link')
    link.href = url
    link.rel = 'stylesheet'
    link.onload = () => {
        resolve()
    }
    link.onerror = (e: Event | string) => {
        const msg = typeof e === 'string' ? e : 'load failed'

        reject(new Error(`Failed to load ${url}: ${msg}`))
    }
    document.body.appendChild(link)
});


export const loadScript = (url: string) => {
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

const loadScripts = (urls: string[]) => Promise.all(urls.map(loadScript))

export default loadScripts