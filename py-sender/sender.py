import os
import random
import sys
import time

import msgpack
import numpy as np
import zmq
from PIL import Image


class ImageReader():

    def __init__(self, folder="images"):
        self._folder = folder

    @property
    def images(self):
        try:
            files = os.listdir(self._folder)
        except FileNotFoundError:
            raise Exception("'%s' directory nof found! Create it and put images in there" % self._folder)
        filtered_files = [f for f in files if f.endswith(".jpg") or f.endswith(".png")]
        images = [self._read_image(os.path.join(self._folder, f)) for f in filtered_files]
        print("ImageReader::images: ", filtered_files)
        return images

    @staticmethod
    def _read_image(image_path):
        import base64
        im = Image.open(image_path)
        img_b64 = base64.b64encode(np.array(im)).decode()
        image = base64.b64decode(img_b64.encode())
        return image


class Sender():

    def __init__(self, proxy, port):
        self._url = "tcp://{}:{}".format(proxy, port)
        self._context = zmq.Context()
        self._socket = self._context.socket(zmq.PUB)

    def conect(self):
        print("Sender::conect: ", self._url)
        self._socket.bind(self._url)

    def send(self, topic, message):
        print("Sender::send: ", topic)
        self._socket.send_multipart([topic, message])

    def disconnect(self):
        self._socket.close()
        self._context.term()


def main():
    proxy = "*"
    port = "9001"
    if len(sys.argv) > 1:
        proxy = sys.argv[1]
    if len(sys.argv) > 2:
        port = sys.argv[2]
        int(port)

    sender = Sender(proxy, port)
    reader = ImageReader()

    sender.conect()

    data = {
        'user': {
            'angle': -0.7229224524674215,
            'throttle': 0.44270997587430183
        },
        'pilot': {
            'angle': -0.7229224524674215,
            'throttle': 0.44270997587430183
        },
        'mode': 'user'
    }

    while True:
        for img in reader.images:
            print("Sending image...")
            data["img"] = img
            sender.send(b"donkeycar.training", msgpack.packb(data, use_bin_type=True))
            time.sleep(1)

    sender.disconnect()

if __name__ == "__main__":
    main()
