#!/usr/bin/env python3

from qtpy import QtWidgets
from qtconsole.rich_jupyter_widget import RichJupyterWidget
from qtconsole.inprocess import QtInProcessKernelManager

import qtconsole
import ipykernel
import jupyter_client
import traitlets
import qtpy
import sys

print(sys.version)
print(qtconsole.__version__)
print(ipykernel.__version__)
print(jupyter_client.__version__)
print(traitlets.__version__)
print(qtpy.__version__)


class ConsoleWidget(RichJupyterWidget):
    """Qt widget with an embedded in-process IPython console."""

    def __init__(self, *args, **kwd):
        super().__init__(*args, **kwd)
        self.set_default_style()

        manager = QtInProcessKernelManager()
        manager.start_kernel(show_banner=False)
        self.kernel_manager = manager
        self.kernel_manager.gui = "qt"

        client = manager.client()
        client.start_channels()
        self.kernel_client = client

    def namespace_inject(self, **kwds):
        #self._kernel_manager.kernel.shell.user_ns[name] = obj
        self.kernel_manager.kernel.shell.push(kwds)
