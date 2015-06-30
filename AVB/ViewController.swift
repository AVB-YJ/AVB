//
//  ViewController.swift
//  AVB
//
//  Created by Konstantin Klitenik on 5/17/15.
//  Copyright (c) 2015 DK Audio. All rights reserved.
//

import Cocoa
import AudioVideoBridging

class ViewController: NSViewController {

    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        send_raw()
    }

    override var representedObject: AnyObject? {
        didSet {
        // Update the view, if already loaded.
        }
    }


}

