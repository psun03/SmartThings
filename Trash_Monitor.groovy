/**
 *  Trash_Monitor.groovy
 *
 *  Copyright 2017 Peter Sun
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 *  in compliance with the License. You may obtain a copy of the License at:
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed
 *  on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License
 *  for the specific language governing permissions and limitations under the License.
 *
 *  Change History:
 *
 *    Date        Who            What
 *    ----        ---            ----
 *    2017-03-01  Peter Sun  Original Creation
 
 */
 
metadata {
	definition (name: "Trash_Monitor", namespace: "psun03", author: "Peter Sun") {
        capability "Configuration"
	}

    simulator {
 
    }

    // Preferences
	preferences {
  }

	// Tile Definitions
	tiles {
        valueTile("type", "device.bin", width: 3, height: 2) {
        state "default", label:'${currentValue}', defaultState: true, backgroundColors: [
            [value: "Trash", color: "#53A7C0"],
            [value: "Recycle", color: "#79b821"]
        ]
    	}
		standardTile("check", "device.configure", width: 3, height: 1, canChangeIcon: true,  decoration: "flat") {
			state("Check", label: "Check", action: "configuration.configure")
		}
		main(["type","check"])
	}
}
def initialize() {
}


def sendEthernet() {
    def params = [
            uri: "http://web_host.com",
            path: "/webservice.php"
        ]

        try {
            httpGet(params) { resp ->
                    resp.headers.each {
                    log.debug "name: ${it.name} | value: ${it.value}"
                }
                def value = resp.data["answer"]
                def status = resp.data["status"] 
                log.debug "value: ${value}"
                log.debug "status: ${status}"
                if(value == "T") {
                    sendEvent(name: "bin", value: "Trash")
                }
                else if(value == "TR") {
                    sendEvent(name: "bin", value: "Recycle")
                }
            }
        }
        catch (e) {
            log.error "something went wrong: $e"
        }
}	

// handle commands

def configure() {
	sendEthernet()
}
