import numpy as np
from tf_agents.environments import py_environment
from tf_agents.specs import array_spec
from tf_agents.trajectories import time_step as ts

class CustomEnv(py_environment.PyEnvironment):

    def __init__(self):
        self._action_spec = array_spec.BoundedArraySpec(
            shape=(), dtype=np.int32, minimum=0, maximum=1, name='action')
        self._observation_spec = array_spec.BoundedArraySpec(
            shape=(4,), dtype=np.float32, minimum=-1, maximum=1, name='observation')
        self._state = np.zeros(4, dtype=np.float32)
        self._episode_ended = False

    def action_spec(self):
        return self._action_spec

    def observation_spec(self):
        return self._observation_spec

    def _reset(self):
        self._state = np.zeros(4, dtype=np.float32)
        self._episode_ended = False
        return ts.restart(self._state)

    def _step(self, action):
        if self._episode_ended:
            return self.reset()

        # Simple logic for state transition and reward calculation
        if action == 0:
            self._state += np.array([0.1, 0.0, 0.0, 0.0], dtype=np.float32)
        else:
            self._state += np.array([0.0, 0.1, 0.0, 0.0], dtype=np.float32)

        if np.any(self._state >= 1.0):
            self._episode_ended = True
            reward = 1.0
        else:
            reward = 0.0

        if self._episode_ended:
            return ts.termination(self._state, reward)
        else:
            return ts.transition(self._state, reward)
