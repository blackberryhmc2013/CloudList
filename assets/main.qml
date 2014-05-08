/*
 * Copyright (c) 2013-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import bb.cascades 1.0
Page {
    Container {
    id: containerID
	    ListView {
            id: pushListView
            objectName: "msgList"
            listItemComponents: [
                ListItemComponent {
                    Container {
                        id: listContainer
                        leftPadding: 10
                        rightPadding: 10
                        bottomMargin: 20.0

                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        
                        Container {
                            layout: StackLayout {

                            }
                            Label {
                                text: ListItemData.id
                                textStyle.color: Color.LightGray
                                textStyle.fontSize: FontSize.XXSmall
                            }
                            Label {
                                text: ListItemData.updateTime
                                textStyle.color: Color.DarkGray
                                textStyle.fontSize: FontSize.XXSmall
                            }
                        }
                        Label {
                            text: ListItemData.body
                            textStyle.fontSize: FontSize.Large
                            leftMargin: 40.0
                            verticalAlignment: VerticalAlignment.Center
                        }
                        Button {
                            horizontalAlignment: HorizontalAlignment.Right
                            preferredWidth: 50.0
                            preferredHeight: 50.0
                            onClicked: {
                                listContainer.ListItem.view.deleteItem(listContainer.ListItem.indexPath);
                            }

                        }
                        Divider {
                        
                        }
                    }

                 }
            ]
            snapMode: SnapMode.LeadingEdge
            stickToEdgePolicy: ListViewStickToEdgePolicy.End
            function deleteItem(index) {
                myApp.deleteItem(index);
                console.log("called delete in qml");
            }
        }
        TextField {
            input.submitKey: SubmitKey.Send
            horizontalAlignment: HorizontalAlignment.Center
            input.submitKeyFocusBehavior: SubmitKeyFocusBehavior.Lose
            input {
                onSubmitted: {
                    // Todo: need this to be a slot but cannot
                    // template the slot parameter!
                    //cList.add(text);
                }
            }
        }
        Button {
            id: refresh
            text: "Refresh"
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center
            onClicked: {
                cList.pullData();
            }
        }
    }
}
