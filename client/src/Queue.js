const Queue = (fn, concurrency = 5) => {
  const PENDING = 0;
  const RUNNING = 1;

  let q = [];
  let running = 0;

  const getTask = () => {
    if (running >= concurrency) {
      return null;
    }

    return q.find((t) => t.state === PENDING);
  };

  const removeTask = (task) => {
    q = q.filter((t) => t !== task);
  };

  const consume = () => {
    const task = getTask();

    if (!task) {
      return;
    }

    running += 1;

    task.state = RUNNING;

    const {
      arg,
      resolve,
      reject,
    } = task;

    const promise = fn(arg);

    promise.then((r) => {
      running -= 1;

      removeTask(task);

      resolve(r);

      consume();
    }, (e) => {
      running -= 1;

      removeTask(task);

      reject(e);

      consume();
    });
  };

  const push = (arg) => new Promise((resolve, reject) => {
    q.push({
      arg,
      state: PENDING,
      resolve,
      reject,
    });

    consume();
  });

  return {
    push,
  };
};

export default Queue;