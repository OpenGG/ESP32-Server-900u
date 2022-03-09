interface Get {
    (url: string, type: 'arraybuffer'): Promise<ArrayBuffer>
    <T>(url: string, type: 'json'): Promise<T>
    (url: string, type: 'text'): Promise<string>
}

export const get: Get =
    <T>(url: string, responseType: 'arraybuffer' | 'json' | 'text') => new Promise<T>((resolve, reject) => {
        const xhr = new XMLHttpRequest();
        xhr.responseType = responseType;
        xhr.open("GET", url, true);
        xhr.send();
        xhr.onreadystatechange = () => {
            if (xhr.readyState !== 4) {
                return
            }
            if (xhr.status !== 200) {
                reject(new Error(`Failed to load: ${url}`))
                return
            }
            resolve(xhr.response);
        };
    })
