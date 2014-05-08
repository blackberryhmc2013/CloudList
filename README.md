CloudList Documentation
=======================

CloudList provides the simplicity of a `Qlist` like interface while interfacing with backend cloud services for data synchronization.
Instantiations of CloudList connected to the internet with the same list identifier will attempt to synchronize their contents.

CloudList depends on the cloudbase.io BlackBerry library licensed under Apache 2.0, and YAJL under the ISC license.

Pre-installation
----------------

CloudList requires use of the BlackBerry Push Service as well as cloudbase.io cloud service. Accounts for both services can be registered without cost.

### BlackBerry Push Service

Applications in development should register for the BlackBerry PUSH Plus service by visiting <https://www.blackberry.com/profile/?eventId=8121>. This service is offered on an evaluation basis and must be renewed after six months.

Additional information about the BlackBerry Push Service is available at <https://developer.blackberry.com/services/push/>. Note that an additional SDK is provided for creating push service server but is not necessary for use of CloudList.

After the account application is complete, an email will be sent with the follow with the account details necessary for using the push service: `Application ID`, `Push Password`, and `PUSH API url`.

### cloudbase.io

An account with cloudbase can be registered at <https://manage.cloudbase.io/register>. CloudList utilizes cloudbase.io’s native BlackBerry library available at <https://github.com/cloudbase-io/CBHelper-BlackBerry10Native/>. At this time, the latest cloudbase.io library is packaged with CloudList.

Registering requires an application name and password. The details necessary to use CloudList are `Application Code`, `Unique Code` (both available on the account dashboard), and the md5 hash of the `Application Password` chosen during creation.

Installation
------------

### Using the CloudList template

To create a new application that uses CloudList, clone the CloudList Github repository and use the provided Momentics project as a starting point for the application. Details for configuring CloudList are provided below in Section .

### Adding CloudList to an existing project

To add CloudList to an existing project clone the CloudList Github repository and copy the following folders into your project directory.

-   Copy `src/cloudList` into your `src` directory.

-   Copy `libs/cloudbase-libs` into your `libs` directory. You may have to create the `libs` directory.

-   Copy `assets/templates` into your `assets` directory.

-   Add `LIBS += -L../libs/cloudbase-libs -lCBHelper -lQtXml` into your projects `.pro` file.



Configuration
-------------

Configuring CloudList requires two steps. First the invocation looks for the push service must be added to the project’s `bar-descriptor.xml` as described below. The `invoke-target id` must be unique across any application installed on a device. The convention is to use reverse style DNS naming starting with the applications `id`.

``` 
<invoke-target id="com.example.YOURAPP.clpush">
  <invoke-target-type>application</invoke-target-type>
  <filter>
     <action>bb.action.PUSH</action>
     <mime-type>application/vnd.push</mime-type>
  </filter>
</invoke-target>
```

The second configuration step is to modify <span>`src/cloudList/CloudListConfig.h`</span> and fill in the account details for cloudbase.io, BlackBerry Push Service, and `invoke-target id`.

Usage
=====

For a class to be used with CloudList it must provide two public functions: `QString YourClass::toString() const` and `YourClass(QString strCode)`. The `toString()` function must provide a JSON or plain text representation of the object’s data. The class constructor must convert that same string representation back into the object.

To create a CloudList use the provided templated constructor as shown

    #include "cloudList/CloudList.h"
    ...
    CloudList<QString>("CL_ID");
    ...

Any instantiation of CloudList with the same `CL_ID` will share data. If separate lists are desired (e.g. if you had an application with multiple distinct playlists) then one way to generate a unique name is to use `QUuid::createUuid()` provided by the QT framework.

The following functions can be used to manipulate items in the list.

-   `void add(T data)`
-   `int size() const`
-   `const T& at(int index) const`
-   `T& operator[] (int index)`
-   `void remove(int index)`

Because an object in the list can be updated at any time externally, the function `void update(int index)` must be used to trigger propagation of the changes to other list instantiations.

When a new object is added to or deleted from the list the `newObject(int index)` or `deletedObject(int index)` signal will be emitted. Note that the object won’t be removed from the list until the slots attached to the `deletedObject` signal are done executing. Similarly when a object is updated the `updatedObject` signal is emitted. These slots can be connected to in the following way

    connect(&myList, SIGNAL(newObject(int)), this, SLOT(onItemAdded(int)));
    connect(&myList, SIGNAL(deletedObject(int)), this, SLOT(onItemDeleted(int)));
	
Limitations
===========

CloudList uses the communication aspects of BlackBerry Push Service and cloudbase.io library both of which require an internet connection to work properly. CloudList makes no attempt to recover from temporary lapses in internet connectivity.

CloudList object updates are processed on a first-come-first-served basis; conflicting updates sent in close succession may be interpreted in the incorrect order. CloudList makes no attempt to resolve conflicts across all list instantiations. Applications that make many edits to the same object in close succession on different devices may become desynchronized.

At the time of this writing, cloudbase-io or its provided library do not support non-ASCII characters. While CloudList attempts to escape non-ASCII characters, words like `Malmö` should be converted to an ASCII-only representation.
	
## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
