switch (React.DOM.querySelector("#root")) {
| Some(element) =>
  let root = React.DOM.Client.createRoot(element);
  React.DOM.Client.render(root, <App />);
| None =>
  Js.Console.error("Failed to start React: couldn't find the #root element")
};
