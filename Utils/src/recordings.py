import csv
from collections import defaultdict, namedtuple
from enum import Enum
from itertools import zip_longest, tee


class AgentInfo:
    __slots__ = ["agent_id", "type", "pos", "orient", "R"]

    def __init__(self, agent_id, type, pos, orient, R):
        self.agent_id = agent_id
        self.type = type
        self.pos = pos
        self.orient = orient
        self.R = R


class Recording:
    __slots__ = ["pos", "steps", "step_times", "xmin", "ymin", "xmax", "ymax"]

    def __init__(self, pos, steps, step_times, xmin, ymin, xmax, ymax):
        self.pos = pos
        self.steps = steps
        self.step_times = step_times
        self.xmin = xmin
        self.ymin = ymin
        self.xmax = xmax
        self.ymax = ymax


class AgentType(Enum):
    Agent = 1,
    Group = 2

    @classmethod
    def read(cls, str):
        return cls.Agent if str == "A" else cls.Group


def grouper(iterable, n, fillvalue=None):
    """Collect data into fixed-length chunks or blocks"""
    # grouper('ABCDEFG', 3, 'x') --> ABC DEF Gxx"
    args = [iter(iterable)] * n
    return zip_longest(*args, fillvalue=fillvalue)


def read_recording(filename):
    result = defaultdict(dict)

    minx, miny = 1000000, 1000000
    maxx, maxy = -1000000, -1000000
    steps = 0
    with open(filename) as csvfile:
        traj_file = csv.reader(csvfile, delimiter=',')
        step_times = []

        for step, row in enumerate(traj_file):
            step_time = float(row[0])
            step_times.append(step_time)
            for agent_id, t, x, y, orientx, orienty, radius in grouper(row[1:], 7):
                info = AgentInfo(int(agent_id), AgentType.read(t), (float(x), float(y)), (float(orientx), float(orienty)), float(radius))
                minx = min(info.pos[0], minx)
                miny = min(info.pos[1], miny)
                maxx = max(info.pos[0], maxx)
                maxy = max(info.pos[1], maxy)

                result[int(agent_id)][step] = info

        for vals in result.values():
            steps = max(steps, len(vals))

    return Recording(result, steps, step_times, xmin=minx, ymin=miny, xmax=maxx, ymax=maxy)