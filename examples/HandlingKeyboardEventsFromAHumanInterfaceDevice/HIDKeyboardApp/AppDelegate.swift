/*
See LICENSE folder for this sample’s licensing information.

Abstract:
Implements the app delegate object, which installs the system extension during the launch cycle.
*/

import Cocoa
import SystemExtensions

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate, OSSystemExtensionRequestDelegate {

    let driverID = "com.example.apple-samplecode.KeyboardDriver"

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Insert code here to initialize your application.
        
        // Activate the driver.
        let request = OSSystemExtensionRequest.activationRequest(forExtensionWithIdentifier: driverID, queue: DispatchQueue.main)
        request.delegate = self
        let extensionManager = OSSystemExtensionManager.shared
        extensionManager.submitRequest(request)
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application.
    }
    
    func request(_ request: OSSystemExtensionRequest, didFinishWithResult result: OSSystemExtensionRequest.Result) {
        Swift.print("Request finished successfully")
    }
    
    func request(_ request: OSSystemExtensionRequest, didFailWithError error: Error) {
        Swift.print("Request failed. \(error)")
    }
    
    func requestNeedsUserApproval(_ request: OSSystemExtensionRequest) {
        Swift.print("Request needs approval.")
    }
    
    func request(_ request: OSSystemExtensionRequest,
                 actionForReplacingExtension existing: OSSystemExtensionProperties,
                 withExtension ext: OSSystemExtensionProperties) -> OSSystemExtensionRequest.ReplacementAction {
        Swift.print("Request needs to replace the extension")
        
        return .replace
    }

}

