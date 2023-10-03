open Belt;

type dog = string;

type state =
  | Loading
  | Error
  | Loaded(array(dog));

type action =
  | DogsFetch
  | DogsFetched(array(dog))
  | DogsFailedToFetch;

module Decode = {
  open Json.Decode;
  let dogs = (json): array(dog) =>
    json |> field("message", array(string)) |> Array.map(_, dog => dog);
};

let reducer = (_state, action) =>
  switch (action) {
  | DogsFetch => Loading
  | DogsFetched(dogs) => Loaded(dogs)
  | DogsFailedToFetch => Error
  };

[@react.component]
let make = () => {
  let (state, send) = React.useReducer(reducer, Loading);

  React.useEffect0(() => {
    open Js.Promise;
    send(DogsFetch);

    Js.log("TWIIIIICE");

    Fetch.fetch("https://dog.ceo/api/breeds/list")
    |> then_(Fetch.Response.json)
    |> then_(json =>
         json |> Decode.dogs |> (dogs => send(DogsFetched(dogs))) |> resolve
       )
    |> catch(_err => Js.Promise.resolve(send(DogsFailedToFetch)))
    |> ignore;

    None;
  });

  <React.StrictMode>
    {switch (state) {
     | Error => <div> {React.string("An error occurred!")} </div>
     | Loading => <div> {React.string("Loading...")} </div>
     | Loaded(dogs) =>
       <div>
         <h1> {React.string("Dogs")} </h1>
         <p> {React.string("Source: ")} </p>
         <a href="https://dog.ceo"> {React.string("https://dog.ceo")} </a>
         <ul>
           {Array.map(dogs, dog => <li key=dog> {React.string(dog)} </li>)
            |> React.array}
         </ul>
       </div>
     }}
  </React.StrictMode>;
};
