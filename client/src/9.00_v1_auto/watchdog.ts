type IListener = (msg: string) => void;

const listeners: IListener[] = [];

export const setup = () => {
    window.addEventListener('error', (e) => {
        const message = `${e.message || e}`;
        listeners.forEach(fn => {
            fn(message);
        })
    }, false);

    window.addEventListener('unhandledrejection', (e) => {
        const message = `${e.reason || e}`;
        listeners.forEach(fn => {
            fn(message);
        })
    }, false);
}

export const listen = (fn: IListener) => {
    listeners.push(fn);
}