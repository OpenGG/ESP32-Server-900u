import evaluate from './evaluate';
import loadScripts, { loadScript } from './loadScripts';
import zLog from './zLog';
import { listen as listenWatchdog, setup as setupWatchdog } from './watchdog';

const main = async () => {
  window.zLog = zLog;

  setupWatchdog();

  listenWatchdog(zLog.error);

  zLog.log('Loading...');

  await loadScript('init.js');

  const {
    zInitConfig,
  } = window;

  if (!zInitConfig) {
    zLog.error('init.js not valid');
    return;
  }

  const {
    scripts,
    exec,
  } = zInitConfig

  await loadScripts(scripts);

  if (exec) {
    for (let i = 0; i < exec.length; i++) {
      const code = exec[i];
      await evaluate(code);
    }
  }
}

main().catch((e) => {
  const message = e instanceof Error ? e.message : `${e}`

  zLog.error(message)
})
