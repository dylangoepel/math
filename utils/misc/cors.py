# calculates e^{2 * pi * phi} using 16 radix-2 complex angle rotation

import gen_constants as constants

zetas = constants.zetaRadix2(32)
def circleCoords(phi):
    rest = phi % 1
    (x, y) = (1.0, 0.0)
    for (a, b) in zetas[1:16]:
        rest *= 2
        if rest >= 1:
            rest -= 1
            (x, y) = (x * a - y * b, x * b + y * a)
    return (x, y)


if __name__ == "__main__":
    import matplotlib.pyplot as plt
    import numpy as np

    cos = np.vectorize(lambda x: circleCoords(x)[0])
    sin = np.vectorize(lambda x: circleCoords(x)[1])

    xs = np.arange(0, 1, 0.001)
    plt.plot(xs, cos(xs), label="$\\cos$")
    plt.plot(xs, sin(xs), label="$\\sin$")
    plt.legend()
    plt.show()
