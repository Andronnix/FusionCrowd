from collections import namedtuple
from itertools import combinations

from recordings import Recording, read_recording
from viz import pairwise
from shapely.geometry import LineString


Collision = namedtuple("Collision", ["step1", "step2", "id1", "id2"])


def get_collisions(rec: Recording):
    collisions = set()
    agent_combos = list(combinations(rec.pos.keys(), 2))

    print("Looking for collisions")
    for step0, step1 in pairwise(range(rec.steps)):
        print("Working on steps: {} - {}".format(step0, step1))

        for a1, a2 in agent_combos:
            if step0 not in rec.pos[a1] or step0 not in rec.pos[a2] or step1 not in rec.pos[a1] or step1 not in rec.pos[a2]:
                continue

            a1_step = LineString([rec.pos[a1][step0].pos, rec.pos[a1][step1].pos])
            a2_step = LineString([rec.pos[a2][step0].pos, rec.pos[a2][step1].pos])

            if a1_step.intersects(a2_step):
                collisions.add(Collision(step0, step1, a1, a2))

    return collisions


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("source_recording")

    args = parser.parse_args()
    collisions = get_collisions(read_recording(args.source_recording))

    print(len(collisions))
