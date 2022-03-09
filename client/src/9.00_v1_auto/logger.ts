const showMessage = (text: string, className = '') => {
    const p = document.createElement('p');
    const t = new Date().toLocaleTimeString();
    p.textContent = `[${t}] ${text}`;
    p.className = className;
    const app = document.getElementById('app')!;
    app.insertBefore(p, app.firstChild);
}

export const error = (text: string) => {
    showMessage(text, 'error');
}
export const log = (text: string) => {
    showMessage(text, 'log');
}