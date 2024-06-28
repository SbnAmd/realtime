import math
from typing import Optional, Union
import numpy as np
import gym
from gym import logger, spaces
from gym.error import DependencyNotInstalled


class CPUEnv(gym.Env[np.ndarray, Union[int, np.ndarray]]):
    max_temperature = 85
    min_temperature = 30
    max_power = 300
    min_power = 0
    max_deadlines = 400
    min_deadlines = 0

    def __init__(self):

        high_bound = np.array(
            [
                self.max_temperature,
                self.max_temperature,
                self.max_temperature,
                self.max_temperature,
                self.max_power,
                self.max_deadlines,
                ],
            dtype=np.int32,
        )

        low_bound = np.array(
            [
                self.min_temperature,
                self.min_temperature,
                self.min_temperature,
                self.min_temperature,
                self.min_power,
                self.min_deadlines,
            ],
            dtype=np.int32,
        )

        self.action_space = spaces.Discrete(12)     # 4 cores * 3 freqs
        self.observation_space = spaces.Box(low_bound, high_bound, dtype=np.int32)
        self.steps_beyond_terminated = None
        self.terminated = False

    def step(self, action):
        err_msg = f"{action!r} ({type(action)}) invalid"
        assert self.action_space.contains(action), err_msg
        assert self.state is not None, "Call reset before using step method."
        self.terminated = False

        # fixme
        # cpu.schedule()
        # self.state = cpu.oserve()  # get 4 core temp, and power and deadlines

        if t1 > self.max_temperature or t1 < self.min_temperature:
            self.terminated = True
        if t2 > self.max_temperature or t2 < self.min_temperature:
            self.terminated = True
        if t3 > self.max_temperature or t3 < self.min_temperature:
            self.terminated = True
        if t4 > self.max_temperature or t4 < self.min_temperature:
            self.terminated = True

        if p > self.max_power or p < self.min_power:
            self.terminated = True

        if d > self.max_deadlines:
            self.terminated = True

        if not self.terminated:
            reward = 1.0
        elif self.steps_beyond_terminated is None:
            # Pole just fell!
            self.steps_beyond_terminated = 0
            reward = 1.0
        else:
            if self.steps_beyond_terminated == 0:
                logger.warn(
                    "You are calling 'step()' even though this "
                    "environment has already returned terminated = True. You "
                    "should always call 'reset()' once you receive 'terminated = "
                    "True' -- any further steps are undefined behavior."
                )
            self.steps_beyond_terminated += 1
            reward = 0.0

        return np.array(self.state, dtype=np.float32), reward, self.terminated, False, {}

    def reset(self, *, seed: Optional[int] = None, options: Optional[dict] = None):
        super().reset(seed=seed)
        # Note that if you use custom reset bounds, it may lead to out-of-bound
        # state/observations.
        low, high = utils.maybe_parse_reset_bounds(
            options, -0.05, 0.05  # default low
        )  # default high
        self.state = self.np_random.uniform(low=low, high=high, size=(4,))
        self.steps_beyond_terminated = None

        if self.render_mode == "human":
            self.render()
        return np.array(self.state, dtype=np.float32), {}