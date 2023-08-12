module.exports = [
  {
    "type": "heading",
    "defaultValue": "Settings"
  },
  {
    "type": "text",
    "defaultValue": "Enter desired time here in 24 hour format."
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Sleeping schedule"
      },
    {
  "type": "slider",
  "messageKey": "lsleep",
  "defaultValue": 16,
  "label": "The hour she wakes up",
  "description": '"Oh, most wonderful of nights."',
  "min": 11,
  "max": 21,
  "step": 1
    },
    {
 "type": "slider",
 "messageKey": "lwake",
 "defaultValue": 8,
 "label": "The hour she goes to rest",
 "description": '"Too tired. Please excuse me."',
 "min": 1,
 "max": 10,
 "step": 1
    },
    {
 "type": "slider",
 "messageKey": "uwake",
 "defaultValue": 23,
 "label": "The hour (you) go to sleep",
 "description": '"As always, I will guard the night."',
 "min": 0,
 "max": 23,
 "step": 1
},
{
  "type": "text",
  "defaultValue": "Your Princess wakes up on evening and goes to bed at morning. It's just the way I designed this watchface, so please don't break it."
},
{
  "type": "toggle",
  "messageKey": "noautism",
  "label": "Disable all interactivity",
  "defaultValue": false
}

    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
