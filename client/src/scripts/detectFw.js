/* globals zFwVer */
const detectFw = () => {
  // Mozilla/5.0 (PlayStation 4 5.05) AppleWebKit/601.2 (KHTML, like Gecko)

  const UA = navigator.userAgent;

  const matches = UA.match(/PlayStation \d\/(\d+\.\d+)/);

  const ver = matches ? matches[1] : 'Unknown';

  zFwVer.textContent = ver;
};

detectFw();
