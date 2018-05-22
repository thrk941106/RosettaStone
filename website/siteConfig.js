const siteConfig = {
  title: 'Hearthstone++',
  tagline: 'Hearthstone simulator using C++ with some reinforcement learning',
  url: 'https://utilForever.github.io',
  baseUrl: '/Hearthstonepp/',

  projectName: 'Hearthstonepp',
  organizationName: 'utilForever',

  headerLinks: [
    {doc: 'doc1', label: 'Docs'},
    {doc: 'doc4', label: 'API'},
    {page: 'help', label: 'Help'},
  ],

  headerIcon: 'img/logo.png',
  footerIcon: 'img/logo.png',
  favicon: 'img/logo.png',

  colors: {
    primaryColor: '#5a74a5',
    secondaryColor: '#d8bc4f',
  },

  /* custom fonts for website */
  /*fonts: {
    myFont: [
      "Times New Roman",
      "Serif"
    ],
    myOtherFont: [
      "-apple-system",
      "system-ui"
    ]
  },*/

  copyright: 'Copyright © ' + new Date().getFullYear() + ' Hearthstonepp',

  highlight: {
    theme: 'default',
  },

  scripts: ['https://buttons.github.io/buttons.js'],

  onPageNav: 'separate',

  ogImage: 'img/docusaurus.png',
  twitterImage: 'img/docusaurus.png',

  repoUrl: 'https://github.com/utilForever/Hearthstonepp',
};

module.exports = siteConfig;
