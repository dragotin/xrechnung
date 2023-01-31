# XRechnung

Some utilities around the XRechnung Standard that is used in Germany and other countries for electronic invoicing.

This repository is young and the tools are in very early stage. It is a start to support this standard from open source. Contributions to this efforts are highly appreciated.

XRechnung documents can be created by the software [Kraft](https://volle-kraft-voraus.de) ([Github repo](https://github.com/dragotin/kraft)).

Samples can be found in the [xrechnung-testsuite repository on projekte.kosit.org](https://projekte.kosit.org/xrechnung/xrechnung-testsuite/-/tree/master/).

## XRView

XRView is a Qt based viewer for the Desktop that renders XRechnung XML documents. It uses the official XSL stylesheets provided in [the repository](https://github.com/itplr-kosit/xrechnung-visualization) of the [Koordinierungsstelle für IT-Standards](https://www.xoev.de/). 

![XRView screenshot](/xrview/screenshot1.png?raw=true&s=200 "Screenshot")

Beside the rendering, it also extracts some important data from the XML and displays them in the overview pane.

### Configuration

To run XRView a *configuration file* has to be manually created first. Create a file in the users home directory in the local configuration directory, usually in `$HOME/.config/xrview/xrview.ini`.

The config file has to look like:

```bash
[saxon]
jar=/real/path/saxon-he-11.4.jar
xslHtml=/real/path/xrechnung-visualization/src/xsl/xrechnung-html.xsl
xslUbl=/real/path/xrechnung-visualization/src/xsl/ubl-invoice-xr.xsl
```

The files need to be downloaded and stored on the computer. They can be downloaded here:

1. Saxon: https://github.com/Saxonica/Saxon-HE/tree/main/11/Java (Please make sure to **only** use `SaxonHE11-4J.zip`)
2. xrechnung-visualization: https://github.com/itplr-kosit/xrechnung-visualization It contains the needed XSL stylesheets.

### Usage

Start the viewer from the command line and pass one or more XML files in XRechnung format on the command line.

Example:
```bash
xrview /real/path/3321-342.xml
```

### Build

XRView is written in C++ using the Qt toolkit and cmake. Build as usual.

