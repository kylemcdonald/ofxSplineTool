# ofxSplineTool is an addon for [openFrameworks](http://openframeworks.cc)

ofxSplineTool provides an interface for controlling a cubic spline. It's a hard fork of [ofxCurvesTool](https://github.com/kylemcdonald/ofxCurvesTool/), which emulates the "Curves" dialog in Photoshop. 

## Enable/Disable

The interface only listens to mouse and keyboard events when it is drawn. It's a little magical, but if you're interested in the details see the implementation for `ofxSplineTool::drawEvent()`.

## Loading/Saving

ofxSplineTool stores data in an anonymous list of lists, using a severely simplified [YAML](https://en.wikipedia.org/wiki/Yml) representation. Call `ofxSplineTool::save()` or `ofxSplineTool::load()` with a `.yml` extension.

## Drawing

If you happen to scale or rotate `ofxSplineTool`, it may get confused about where your mouse is clicking, but it's ok to translate it however you like.