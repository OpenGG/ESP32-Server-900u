import evaluate from './evaluate';
import { loadScript } from './loadScripts';
import { error, log } from './logger';
import { listen, setup } from './watchdog';

const main = async () => {
  setup();
  listen(error);

  log('Loading...');

  await loadScript('init.js');

  const {
    zInitConfig,
  } = window as unknown as any;

  if (!zInitConfig) {
    error('init.js not valid');
    return;
  }
  const {
    scripts,
    exec,
  } = zInitConfig

  if (scripts) {
    for (let i = 0; i < scripts.length; i++) {
      const url = scripts[i];

      await loadScript(url);
    }
  }

  if (exec) {
    for (let i = 0; i < exec.length; i++) {
      const code = exec[i];
      await evaluate(code);
    }
  }
}

main().catch((e: Error) => {
  const message = e instanceof Error ? e.message : `${e}`
  error(message)
})
