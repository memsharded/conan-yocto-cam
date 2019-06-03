import zmq
import os
import time
import random
import sys


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
        f = open(image_path,'rb')
        bytes = bytearray(f.read())
        string = base64.b64encode(bytes)
        f.close()
        return string


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
    port = "5559"
    if len(sys.argv) > 1:
        proxy = sys.argv[1]
    if len(sys.argv) > 2:
        port = sys.argv[2]
        int(port)

    sender = Sender(proxy, port)
    reader = ImageReader()

    sender.conect()

    while True:
        for img in reader.images:
            print("Sending image...")
            sender.send(b"donkeycar.camera", img)
            time.sleep(1)

    sender.disconnect()

if __name__ == "__main__":
    main()
