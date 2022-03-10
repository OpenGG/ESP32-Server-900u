const showMessage = (text, className = '') => {
    const p = document.createElement('p');
    const t = new Date().toLocaleTimeString();
    p.textContent = `[${t}] ${text}`;
    p.className = className;
    const app = document.getElementById('app');
    app.insertBefore(p, app.firstChild);
}

const zLog = {
    error(text) {
        showMessage(text, 'error');
    },
    log(text) {
        showMessage(text, 'log');
    },
}

export default zLog;