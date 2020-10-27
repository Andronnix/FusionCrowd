import math
from collections import namedtuple, defaultdict
from itertools import combinations
from numbers import Number

from recordings import Recording, read_recording
from viz import pairwise


Collision = namedtuple("Collision", ["step1", "step2", "id1", "id2"])


class Point:
    __slots__ = ["x", "y"]

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def sqr(self):
        return self.x * self.x + self.y * self.y

    def dot(self, other):
        return self.x * other.x + self.y * other.y

    def __neg__(self):
        return Point(-self.x, -self.y)

    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Point(self.x - other.x, self.y - other.y)

    def __mul__(self, other):
        if isinstance(other, Number):
            return Point(self.x * other, self.y * other)

def sub(p1, p2):
    return Point(p1[0] - p2[0], p1[1] - p2[1])


def ray_circle_ttc(d, center2, R):
    a = d.sqr()

    if abs(a) < 1e-8:
        return None

    b = -2 * (d.dot(center2))
    c = center2.sqr() - R*R
    discr = b * b - 4 * a * c

    if discr < 0:
        return None

    sqrtDiscr = math.sqrt(discr)
    t0 = (-b - sqrtDiscr) / (2 * a)
    t1 = (-b + sqrtDiscr) / (2 * a)

    if t1 < t0:
        t0, t1 = t1, t0

    t0 = max(0, t0)
    if t0 > 1 or t1 < t0:
        return None

    return t0, min(t1, 1)


def get_collisions(rec: Recording):
    collisions = set()
    time_in_collision = defaultdict(lambda: 0)
    agent_combos = list(combinations(rec.pos.keys(), 2))

    print("Looking for collisions")
    for step0, step1 in pairwise(range(rec.steps)):
        print("Working on steps: {} - {}".format(step0, step1))

        dt = rec.step_times[step1] - rec.step_times[step0]

        for a1, a2 in agent_combos:
            rec_pos_a1 = rec.pos[a1]
            rec_pos_a2 = rec.pos[a2]

            if step0 not in rec_pos_a1 or step0 not in rec_pos_a2 or \
                    step1 not in rec_pos_a1 or step1 not in rec_pos_a2:
                continue

            v1 = sub(rec_pos_a1[step1].pos, rec_pos_a1[step0].pos)
            v2 = sub(rec_pos_a2[step1].pos, rec_pos_a2[step0].pos)

            dv = v1 - v2
            c = sub(rec_pos_a1[step0].pos, rec_pos_a2[step0].pos)

            collision = ray_circle_ttc(dv, c, 2 * 0.19)

            if collision is None:
                continue

            collisions.add(Collision(step0, step1, a1, a2))
            time_in_collision[a1] += (collision[1] - collision[0]) * dt
            time_in_collision[a2] += (collision[1] - collision[0]) * dt

    return collisions, time_in_collision


# C:\projects\phd\FusionCrowd\TestFusionCrowd\Runs\20201021\Karamouzas_20_trajs.csv
# C:\projects\phd\FusionCrowd\TestFusionCrowd\Runs\20201018\ORCA_0_trajs.csv
if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("source_recording")

    args = parser.parse_args()
    collisions, time_in_collision = get_collisions(read_recording(args.source_recording))

    with open(args.source_recording + "_collision_time.csv", "w") as fout:
        result = sorted(time_in_collision.items(), key=lambda at: at[0])

        fout.write("agent,total_collision_time\n")
        for at in result:
            fout.write("{},{}\n".format(*at))
